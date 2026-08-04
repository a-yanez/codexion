/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 15:58:24 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/20 14:46:48 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "codexion.h"
#include "utils/utils.h"

/*
The "data" array in args contains the data for the following
parameters:
1. number of coders - idx 0
2. time to burnout - idx 1
3. time to compile - idx 2
4. time to debug - idx 3
5. time to refactor - idx 4
6. number of compiles required - idx 5
7. dongle cooldown - idx 6
 */

static int	done_n_deltat(t_args *args, t_coder *coder)
{
	int64_t	delta_t;
	int		done;

	done = is_coder_done(coder);
	if (done < 0)
		return (-1);
	else if (done)
		return (0);
	delta_t = calculate_delta(coder, args->ref_t[1]);
	if (delta_t < 0)
		return (-1);
	else if (delta_t >= args->data[1])
	{
		args->burnt_coder = coder->n_id;
		if (safe_mutex_lock(&args->begin_mtx))
			return (-1);
		args->poison = 1;
		if (safe_mutex_unlock(&args->begin_mtx, 0))
			return (-1);
		return (1);
	}
	return (0);
}

static int	burnout(t_args *args, t_coder *coders)
{
	int	i;
	int	signal;

	i = 0;
	while (i < args->data[0])
	{
		signal = done_n_deltat(args, &coders[i]);
		if (signal < 0)
			return (-1);
		else if (signal)
			return (1);
		i++;
	}
	return (0);
}

static int	inner_loop(t_args *args, int *working)
{
	int		signal;
	t_coder	*coders;

	coders = args->coders;
	if (safe_gettimeofday(&(args->ref_t[1])))
		return (-1);
	signal = burnout(args, coders);
	if (signal == 1)
		return (1);
	else if (signal < 0)
		return (-1);
	*working = coders_working(args);
	if (*working < 0)
		return (-1);
	return (0);
}

static int	print_burnout(t_args *args)
{
	t_dongle	*dongles;
	int64_t		b_time;
	int			i;

	if (args->burnt_coder)
	{
		dongles = args->dongles;
		i = 0;
		while (i < args->data[0])
		{
			if (safe_mutex_lock(&dongles[i].lock))
				return (-1);
			if (safe_cond_broadcast(&args->dongles[i].cond))
				return (safe_mutex_unlock(&dongles[i].lock, 1));
			if (safe_mutex_unlock(&dongles[i].lock, 0))
				return (-1);
			i++;
		}
		b_time = t_diff(args->ref_t[1], args->ref_t[0]);
		printf("%lld %d burned out\n", (long long)b_time, args->burnt_coder);
	}
	return (0);
}

void	*monitor_routine(void *args)
{
	t_args		*ar;
	int			signal;
	int			working;

	ar = (t_args *)args;
	if (wait(&ar->begin_mtx, &ar->begin_cnd, &ar->coder_ready, ar->data[0]))
		return (NULL);
	if (safe_gettimeofday(&(ar->ref_t[1])))
		return (NULL);
	working = coders_working (ar);
	if (working < 0)
		return (NULL);
	while (working)
	{
		signal = inner_loop(ar, &working);
		if (signal < 0)
			return (NULL);
		else if (signal)
			break ;
	}
	print_burnout(ar);
	return (NULL);
}

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
The data array contains the data for the following
parameters:
1. number of coders - idx 0
2. time to burnout - idx 1
3. time to compile - idx 2
4. time to debug - idx 3
5. time to refactor - idx 4
6. number of compiles required - idx 5
7. dongle cooldown - idx 6
 */

int	wait(pthread_mutex_t *m, pthread_cond_t *c, int *ready, int num)
{
	if (safe_mutex_lock(m))
		return (1);
	*ready += 1;
	if (*ready < num + 1)
	{
		while (*ready < num + 1)
		{
			if (safe_cond_wait(c, m))
				return (safe_mutex_unlock(m, 1));
		}
	}
	else
	{
		if (safe_cond_broadcast(c))
			return (safe_mutex_unlock(m, 1));
	}
	if (safe_mutex_unlock(m, 0))
		return (1);
	return (0);
}

static int	burnout(t_args *args, t_coder *coders)
{
	int				i;
	suseconds_t		burnout;
	long			t_delta;

	i = 0;
	burnout = args->data[1];
	while (i < args->data[0])
	{
		t_delta = calculate_delta(&coders[i], args->ref_t[1]);
		if (t_delta < 0)
			return (-1);
		if (t_delta >= burnout)
		{
			args->burnt_coder = coders[i].n_id;
			if (safe_mutex_lock(&args->begin_mtx))
				return (-1);
			args->poison = 1;
			if (safe_mutex_unlock(&args->begin_mtx, 0))
				return (-1);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	print_burnout(t_args *args)
{
	t_dongle	*dongles;
	long		b_time;
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
		printf("%ld %d burned out\n", b_time, args->burnt_coder);
	}
	return (0);
}

void	*monitor_routine(void *args)
{
	t_args		*ar;
	t_coder		*coders;
	int			signal;

	ar = (t_args *)args;
	coders = ar->coders;
	if (wait(&ar->begin_mtx, &ar->begin_cnd, &ar->coder_ready, ar->data[0]))
		return (NULL);
	if (safe_gettimeofday(&(ar->ref_t[1])))
		return (NULL);
	while (coders_working(ar))
	{
		if (safe_gettimeofday(&(ar->ref_t[1])))
			return (NULL);
		signal = burnout(ar, coders);
		if (signal == 1)
			break ;
		else if (signal < 0)
			return (NULL);
	}
	print_burnout(ar);
	return (NULL);
}

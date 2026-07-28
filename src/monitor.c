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

int	wait(pthread_mutex_t *m, pthread_cond_t *c, int *ready, int *num)
{
	if (safe_mutex_lock(m))
		return (1);
	*ready += 1;
	printf("%d coders or monitor checked in...\n", *ready);
	if (*ready < (*num + 1))
	{
		printf("waiting...\n");
		while (*ready < (*num + 1))
		{
			if (safe_cond_wait(c, m))
				return (safe_mutex_unlock(m));
		}
	}
	else
	{
		printf("Everyone is ready!\n");
		if (safe_cond_broadcast(c))
			return (safe_mutex_unlock(m));
	}
	return (safe_mutex_unlock(m));
}

static int	coders_working(t_args *args)
{
	return (args->coder_done >= 0 && args->coder_done < args->data[0]);
}

static int	burnout(t_args *args, t_coder *coders)
{
	int				i;
	suseconds_t		burnout;
	struct timeval	t;

	i = 0;
	t = args->ref_t[1];
	burnout = args->data[1];
	while (i < args->data[0])
	{
		if (t_diff(t, coders[i].last_compile_start) >= burnout)
		{
			args->burnt_coder = coders[i].n_id;
			if (safe_mutex_lock(&args->begin_mtx))
				return (-1);
			args->poison = 1;
			if (safe_mutex_unlock(&args->begin_mtx))
				return (-1);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	print_burnout(t_args *args)
{
	struct timeval	ref;
	struct timeval	t;
	int				coder_id;
	long			b_time;
	int				i;

	i = 0;
	while (i < args->data[0])
	{
		pthread_cond_broadcast(&args->dongles[i].cond);
		i++;
	}
	ref = args->ref_t[0];
	t = args->ref_t[1];
	coder_id = args->burnt_coder;
	b_time = t_diff(t, ref);
	printf("%ld %d burned out\n", b_time, coder_id);
}

void	*monitor_routine(void *args)
{
	t_args		*ar;
	t_coder		*coders;
	int			signal;

	ar = (t_args *)args;
	coders = ar->coders;
	if (wait(&ar->begin_mtx, &ar->begin_cnd, &ar->coder_ready, &ar->data[0]))
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
	if (ar->burnt_coder)
		print_burnout(ar);
	return (NULL);
}

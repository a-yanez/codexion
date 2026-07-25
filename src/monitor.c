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
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include "codexion.h"
#include "utils/utils.h"
#include <stdio.h>

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

static int	coders_working(t_args *args)
{
	return (args->coder_ready >= 0 && args->coder_ready < args->data[0]);
}

static int	burnout(t_args **args, t_coder *coders)
{
	int				i;
	suseconds_t		burnout;
	struct timeval	t;

	i = 0;
	t = (*args)->ref_t[1];
	burnout = (*args)->data[1];
	while (i < (*args)->data[0])
	{
		if (t_diff(t, coders[i].last_compile_start) >= burnout)
		{
			(*args)->burnt_coder = coders[i].n_id;
			(*args)->poison = 1;
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

	ar = (t_args *)args;
	coders = ar->coders;
	if (wait(&ar->begin_mtx, &ar->begin_cnd, &ar->coder_ready, &ar->data[0]))
		return (NULL);
	while (coders_working(ar))
	{
		gettimeofday(&(ar->ref_t[1]), NULL);
		if (burnout(&ar, coders))
			break ;
	}
	if (ar->burnt_coder)
		print_burnout(ar);
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 15:58:24 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/08 15:58:27 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
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

static int	init_cond_mtx(t_args *args, t_dongle *dongles)
{
	int	i;

	if (pthread_cond_init(&args->begin_cnd, NULL))
		return (0);
	if (pthread_mutex_init(&args->begin_mtx, NULL))
		return (0);
	i = 0;
	while (i < args->data[0])
	{
		printf("Activating cond init in dongle %p\n", &dongles[i]);
		if (pthread_cond_init(&dongles[i].cond, NULL))
			return (0);
		if (pthread_mutex_init(&dongles[i].lock, NULL))
			return (0);
		i++;
	}
	return (1);
}

static void	pass_the_ref(t_coder **coders, struct timeval *ref, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		coders[i]->ref = ref;
		i++;
	}
}

static t_c_args	*create_c_args(t_args *args, t_coder *coders, struct timeval *t)
{
	int			i;
	t_c_args	*c_args;

	c_args = (t_c_args *)malloc(sizeof(t_c_args) * args->data[0]);
	if (!c_args)
		return (NULL);
	i = 0;
	while (i < args->data[0])
	{
		c_args[i].coder = &coders[i];
		c_args[i].t = t;
		c_args[i].coder_num = &args->data[0];
		c_args[i].coder_ready = &args->coder_ready;
		c_args[i].begin_mtx = &args->begin_mtx;
		c_args[i].begin_cnd = &args->begin_cnd;
		i++;
	}
	return (c_args);
}

static int	coders_working(t_args *args)
{
	return (args->coder_ready > 0 && args->coder_ready < args->data[0]);
}

void	*run_codexion(void *args)
{
	int				i;
	struct timeval	ref[2];
	t_coder			*coders;
	t_dongle		*dongles;
	t_c_args		*c_args;

	i = init_wrapper(&coders, &dongles, (t_args *) args);
	if (!i || !init_cond_mtx(((t_args *)args), dongles))
	{
		free_dongles(&dongles, ((t_args *)args)->data[0] - 1);
		free_coders(&coders, ((t_args *)args)->data[0] - 1);
		return (NULL);
	}
	i = 0;
	while (i < ((t_args *)args)->data[0])
	{
		pthread_create(&coders[i].thread_id, NULL, coder_rutine, &c_args[i]);
		i++;
	}
	gettimeofday(&ref[0], NULL);
	barrier_wait(&c_args[0]);
	while (coders_working((t_args *)args))
		gettimeofday(&ref[1], NULL);
	return (NULL);
}

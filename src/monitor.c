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

static t_c_args	*create_c_args(t_args *args, t_coder *coders, t_dongle *dongles)
{
	int			i;
	t_c_args	*c_args;

	c_args = (t_c_args *)malloc(sizeof(t_c_args) * args->data[0]);
	if (!c_args)
	{
		destroy_mutex(args, dongles, args->data[0] - 1);
		destroy_conds(args, dongles, args->data[0] - 1);
		free_both(&coders, &dongles, args->data[0] - 1);
		return (NULL);
	}
	i = 0;
	while (i < args->data[0])
	{
		coders[i].ref = &args->ref_t[0];
		c_args[i].coder = &coders[i];
		c_args[i].t = &args->ref_t[1];
		c_args[i].coder_num = &args->data[0];
		c_args[i].coder_ready = &args->coder_ready;
		c_args[i].begin_mtx = &args->begin_mtx;
		c_args[i].begin_cnd = &args->begin_cnd;
		i++;
	}
	return (c_args);
}

static int	set_the_table(t_coder **coders, t_dongle **dongles, t_args *args)
{
	if (!init_wrapper(coders, dongles, args))
		return (0);
	if (!init_cond(args, *dongles))
	{
		free_both(coders, dongles, args->data[0] - 1);
		return (0);
	}
	if (!init_mutex(args, *dongles))
	{
		destroy_conds(args, *dongles, args->data[0] - 1);
		free_both(coders, dongles, args->data[0] - 1);
		return (0);
	}
	return (1);
}

void	pass_the_ref(t_args *args, t_coder *coders)
{
	int				i;
	struct timeval	ref;

	i = 0;
	ref = args->ref_t[0];
	while (i < args->data[0])
	{
		coders[i].poison = &args->poison;
		coders[i].last_compile_start.tv_sec = ref.tv_sec;
		coders[i].last_compile_start.tv_usec = ref.tv_usec;
		i++;
	}
}

void	*run_codexion(void *args)
{
	int				i;
	t_coder			*coders;
	t_dongle		*dongles;
	t_c_args		*c_args;

	if (!set_the_table(&coders, &dongles, (t_args *)args))
		return (NULL);
	c_args = create_c_args((t_args *)args, coders, dongles);
	if (!c_args)
		return (NULL);
	i = 0;
	gettimeofday(&((t_args *)args)->ref_t[0], NULL);
	while (i < ((t_args *)args)->data[0])
	{
		pthread_create(&coders[i].thread_id, NULL, coder_rutine, &c_args[i]);
		i++;
	}
	pass_the_ref(((t_args *)args), coders);
	barrier_wait(&c_args[0]);
	while (coders_working((t_args *)args) && ((t_args *)args)->poison == 0)
	{
		gettimeofday(&(((t_args *)args)->ref_t[1]), NULL);
		if (burnout((t_args *)args, coders))
		{
			print_action(&coders[(((t_args *)args)->burnt_coder)], "burnout", &(((t_args *)args)->ref_t[1]));
			break;
		}
	}
	return (NULL);
}

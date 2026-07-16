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
#include <sys/time.h>
#include <pthread.h>
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

int	init_cond(t_dongle *dongles, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		printf("Activating cond init in dongle %p\n", &dongles[i]);
		if (pthread_cond_init(&dongles[i].cond, NULL))
			return (0);
		i++;
	}
	return (1);
}

void	pass_the_ref(t_coder **coders, struct timeval *ref, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		coders[i]->ref = ref;
		i++;
	}
}

t_coder_args	*create_c_args(t_args *args, t_coder *coders, struct timeval *t)
{
	int				i;
	t_coder_args	*c_args;

	c_args = (t_coder_args *)malloc(sizeof(t_coder_args) * args->data[0]);
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

void	*run_codexion(void *args)
{
	int				i;
	struct timeval	ref[2];
	t_coder			*coders;
	t_dongle		*dongles;
	t_coder_args	*c_args;

	i = init_wrapper(&coders, &dongles, (t_args *) args);
	if (!init_cond(dongles, ((t_args *)args)->data[0]))
	{
		free_dongles(&dongles, ((t_args *)args)->data[0] - 1);
		free_coders(&coders, ((t_args *)args)->data[0] - 1);
		return (NULL);
	}
	gettimeofday(&ref[0], NULL);
	i = 1;
	while (i)
	{
		//something
	}
	// NEXT IDEA! Using a cond variable to indicate that i is now false?
	// Maybe the coders can access it?
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:48:51 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/06 12:48:53 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

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

static t_dongle	*dongle_init(int *data, char *sched)
{
	struct s_dongle	*dongles;
	int				i;
	int				k;

	k = 0;
	if (strcmp(sched, "edf") == 0)
		k = 1;
	dongles = (t_dongle *)malloc(sizeof(t_dongle) * data[0]);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < data[0])
	{
		dongles[i].cool_down = data[6] * 1000;
		dongles[i].on_use = 0;
		dongles[i].queue[0] = NULL;
		dongles[i].queue[1] = NULL;
		dongles[i].edf = k;
		i++;
	}
	return (dongles);
}

static t_coder	*coder_init(int *data)
{
	struct s_coder	*coders;
	int				i;

	coders = (t_coder *)malloc(sizeof(t_coder) * data[0]);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < data[0])
	{
		coders[i].n_id = i + 1;
		coders[i].burnout = data[1] * 1000;
		coders[i].compt_time = data[2] * 1000;
		coders[i].db_time = data[3] * 1000;
		coders[i].refac_time = data[4] * 1000;
		coders[i].cycles = data[5];
		i++;
	}
	return (coders);
}

static void	assign_dongles(t_coder *coder, t_dongle **dongles, int i, int num)
{
	int	k;

	coder->dongles[0] = &((*dongles)[i]);
	k = (i + 1) % num;
	if (&((*dongles)[i]) == &((*dongles)[k]))
		coder->dongles[1] = NULL;
	else
		coder->dongles[1] = &((*dongles)[k]);
	if (i % 2 == 0)
		ft_pswap((void **)&coder->dongles[0], (void **)&coder->dongles[1]);
}

int	init_wrapper(t_coder **coders, t_dongle **dongles, t_args *args)
{
	int				i;

	*coders = coder_init(((t_args *)args)->data);
	if (!pthread_mutex_init(&((t_args *)args)->printer, NULL))
		return (0);
	if (!(*coders))
	{
		pthread_mutex_destroy(&((t_args *)args)->printer);
		return (0);
	}
	*dongles = dongle_init(((t_args *)args)->data, ((t_args *)args)->sched);
	if (!(*dongles))
	{
		pthread_mutex_destroy(&((t_args *)args)->printer);
		free_coders(coders, ((t_args *)args)->data[0] - 1);
		return (0);
	}
	i = 0;
	while (i < ((t_args *)args)->data[0])
	{
		(*coders)[i].printer = &((t_args *)args)->printer;
		assign_dongles(&((*coders)[i]), dongles, i, ((t_args *)args)->data[0]);
		i++;
	}
	return (1);
}

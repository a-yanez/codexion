/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_n_coder_creation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:48:51 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/25 02:06:14 by ayanez-o         ###   ########.fr       */
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

static t_dongle	*dongle_init(t_args *args)
{
	struct s_dongle	*dongles;
	int				*data;
	int				i;

	data = args->data;
	dongles = (t_dongle *)malloc(sizeof(t_dongle) * data[0]);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < data[0])
	{
		dongles[i].cool_down = data[6];
		dongles[i].on_use = 0;
		dongles[i].queue[0] = NULL;
		dongles[i].queue[1] = NULL;
		dongles[i].edf = data[7];
		i++;
	}
	return (dongles);
}

static void	assign_dongles(t_coder *coder, t_dongle *dongles, int i, int num)
{
	int			k;

	coder->dongles[0] = &(dongles[i]);
	k = (i + 1) % num;
	if (&(dongles[i]) == &(dongles[k]))
		coder->dongles[1] = NULL;
	else
		coder->dongles[1] = &(dongles[k]);
	if (i % 2 == 0)
		ft_pswap((void **)&coder->dongles[0], (void **)&coder->dongles[1]);
}

static t_coder	*coder_init(t_args *args)
{
	t_coder		*coders;
	t_dongle	*dongles;
	int			i;

	dongles = args->dongles;
	coders = (t_coder *)malloc(sizeof(t_coder) * args->data[0]);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < args->data[0])
	{
		coders[i].n_id = i + 1;
		coders[i].cycles = args->data[5];
		coders[i].comp_times = 0;
		coders[i].holding = 0;
		coders[i].finished = 0;
		coders[i].poison = &args->poison;
		coders[i].compt_time = args->data[2] * 1000;
		coders[i].db_time = args->data[3] * 1000;
		coders[i].refac_time = args->data[4] * 1000;
		assign_dongles(&(coders[i]), dongles, i, args->data[0]);
		coders[i].printer = &args->printer;
		i++;
	}
	return (coders);
}

int	dongle_n_coder_creator(t_args *args)
{
	t_coder			*coders;
	t_dongle		*dongles;

	dongles = dongle_init(args);
	if (!dongles)
		return (1);
	args->dongles = dongles;
	coders = coder_init(args);
	if (!coders)
		return (1);
	args->coders = coders;
	return (0);
}

t_c_args	*create_c_args(t_args *args)
{
	int			i;
	t_coder		*coders;
	t_c_args	*c_args;

	coders = args->coders;
	c_args = (t_c_args *)malloc(sizeof(t_c_args) * args->data[0]);
	if (!c_args)
		return (NULL);
	i = 0;
	while (i < args->data[0])
	{
		c_args[i].coder = &coders[i];
		c_args[i].coder_num = args->data[0];
		c_args[i].burnout = args->data[1];
		c_args[i].coder_ready = &args->coder_ready;
		c_args[i].coder_done = &args->coder_done;
		c_args[i].begin_mtx = &args->begin_mtx;
		c_args[i].begin_cnd = &args->begin_cnd;
		i++;
	}
	return (c_args);
}

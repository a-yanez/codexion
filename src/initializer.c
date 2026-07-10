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
#include <stdlib.h>
#include <string.h>
#include <string.h>
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

static t_dongle	*dongle_init(int *data)
{
	struct s_dongle	*dongles;
	int				i;
	int				k;

	dongles = (t_dongle *)malloc(sizeof(t_dongle) * data[0]);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < data[0])
	{
		dongles[i].cool_down = data[6];
		dongles[i].avail = 1;
		memset(&dongles[i].queue, 0, 2);
		k = gettimeofday(&(dongles[i].time), NULL);
		if (k < 0)
		{
			free_dongles(&dongles, i - 1);
			return (NULL);
		}
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
		coders[i].cycles = data[4];
		i++;
	}
	return (coders);
}

void	assing_dongles(t_coder *coder, t_dongle **dongles, int i, int num)
{
	int	k;

	coder->dongle_left = &((*dongles)[i]);
	k = (i + 1) % num;
	if (&((*dongles)[i]) == &((*dongles)[k]))
		coder->dongle_right = NULL;
	else
		coder->dongle_right = &((*dongles)[k]);
}

int	init_wrapper(t_coder **coders, t_dongle **dongles, int *data)
{
	int	i;

	*coders = coder_init(data);
	if (!(*coders))
		return (0);
	*dongles = dongle_init(data);
	if (!(*dongles))
	{
		free_coders(coders, data[0] - 1);
		return (0);
	}
	i = 0;
	while (i < data[0])
	{
		assing_dongles(&((*coders)[i]), dongles, i, data[0]);
		(*coders)[i].compt_time = data[2];
		(*coders)[i].db_time = data[3];
		(*coders)[i].refac_time = data[4];
		i++;
	}
	return (1);
}

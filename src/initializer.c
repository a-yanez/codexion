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
#include <stdlib.h>
#include <stdio.h>
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

void	free_dongles(t_dongle	**dongles, int current)
{
	while (current >= 0)
	{
		free(&((*dongles)[current]));
		current--;
	}
}

void	free_coders(t_coder **coders, int current)
{
	while (current >= 0)
	{
		free(&((*coders)[current]));
		current--;
	}
}

t_dongle	*dongle_init(int *data)
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

t_coder	*coder_init(int *data)
{
	struct s_coder	*coders;
	int				i;

	coders = (t_coder *)malloc(sizeof(t_coder) * data[0]);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < data[0])
	{
		coders[i].n_id = i;
		coders[i].cycles = data[4];
		i++;
	}
	return (coders);
}

int	init_wrapper(t_coder **coders, t_dongle **dongles, int *data)
{
	int	i;
	int	k;

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
		(*coders)[i].dongle_left = &((*dongles)[i]);
		k = (i + 1) % data[0];
		(*coders)[i].dongle_right = &((*dongles)[k]);
		i++;
	}
	return (1);
}

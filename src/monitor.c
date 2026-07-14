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

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "codexion.h"
#include "utils/utils.h"

int	check_avail(t_dongle *dongle)
{
	struct timeval	time_measure;

	gettimeofday(&time_measure, NULL);
	if (time_measure.tv_usec - dongle->last_used > dongle->cool_down)
	{
		dongle->on_use = 0;
	}
	return (1);
}

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

void	*run_codexion(void *args)
{
	int				i;
	int				*data;
	struct s_coder	*coders;
	struct s_dongle	*dongles;

	data = ((t_args *)args)->data;
	i = init_wrapper(&coders, &dongles, (t_args *) args);
	if (i)
	{
		printf("Coders and dongles initialized correctly\n");
		printf("The right dongle of coder # %d is %p\n", coders[data[0] - 1].n_id, coders[data[0] - 1].dongles[0]);
		printf("The left dongle of code # %d coder is %p\n", coders[0].n_id, coders[0].dongles[1]);
	}
	if (!init_cond(dongles, data[0]))
	{
		free_dongles(&dongles, data[0] - 1);
		free_coders(&coders, data[0] - 1);
		return (NULL);
	}
	// NEXT IDEA! Using a cond variable to indicate that i is now false? Maybe the coders can access it?
	return (NULL);
}

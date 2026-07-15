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

void	*run_codexion(void *args)
{
	int				i;
	struct timeval	ref;
	struct s_coder	*coders;
	struct s_dongle	*dongles;

	i = init_wrapper(&coders, &dongles, (t_args *) args);
	if (!init_cond(dongles, ((t_args *)args)->data[0]))
	{
		free_dongles(&dongles, ((t_args *)args)->data[0] - 1);
		free_coders(&coders, ((t_args *)args)->data[0] - 1);
		return (NULL);
	}
	gettimeofday(&ref, NULL);
	// NEXT IDEA! Using a cond variable to indicate that i is now false? Maybe the coders can access it?
	return (NULL);
}

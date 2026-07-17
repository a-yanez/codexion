/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conds_n_mutex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 14:21:13 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/17 14:21:41 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

int	destroy_conds(t_args *args, t_dongle *dongles, int i)
{
	pthread_cond_destroy(&args->begin_cnd);
	while (i >= 0)
	{
		pthread_cond_destroy(&dongles[i].cond);
		i--;
	}
	return (0);
}

int	init_cond(t_args *args, t_dongle *dongles)
{
	int	i;

	if (pthread_cond_init(&args->begin_cnd, NULL))
		return (0);
	i = 0;
	while (i < args->data[0])
	{
		if (pthread_cond_init(&dongles[i].cond, NULL))
			return (destroy_conds(args, dongles, i - 1));
		i++;
	}
	return (1);
}

int	destroy_mutex(t_args *args, t_dongle *dongles, int i)
{
	pthread_mutex_destroy(&args->begin_mtx);
	pthread_mutex_destroy(&args->printer);
	while (i >= 0)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		i--;
	}
	return (0);
}

int	init_mutex(t_args *args, t_dongle *dongles)
{
	int	i;

	if (pthread_mutex_init(&args->begin_mtx, NULL))
		return (0);
	i = 0;
	while (i < args->data[0])
	{
		if (pthread_mutex_init(&dongles[i].lock, NULL))
			return (destroy_mutex(args, dongles, i - 1));
		i++;
	}
	return (1);
}

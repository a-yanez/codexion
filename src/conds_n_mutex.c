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

int	destroy_conds(t_args *args)
{
	int			i;
	int			init_cond;
	t_dongle	*dongles;

	dongles = args->dongles;
	init_cond = args->cnd_init;
	pthread_cond_destroy(&args->begin_cnd);
	init_cond--;
	i = 0;
	while (i < init_cond)
	{
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	return (0);
}

int	init_cond(t_args *args)
{
	int			i;
	t_dongle	*dongles;

	dongles = args->dongles;
	if (pthread_cond_init(&args->begin_cnd, NULL))
		return (1);
	args->cnd_init += 1;
	i = 0;
	while (i < args->data[0])
	{
		if (pthread_cond_init(&dongles[i].cond, NULL))
			return (destroy_conds(args));
		args->cnd_init += 1;
		i++;
	}
	return (0);
}

int	destroy_mutex(t_args *args)
{
	int			i;
	int			init_mutex;
	t_dongle	*dongles;

	dongles = args->dongles;
	init_mutex = args->mtx_init;
	pthread_mutex_destroy(&args->begin_mtx);
	if (init_mutex > 1)
		pthread_mutex_destroy(&args->printer);
	else
		return (0);
	init_mutex -= 2;
	i = 0;
	while (i < init_mutex)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		i++;
	}
	return (0);
}

int	init_mutex(t_args *args)
{
	int			i;
	t_dongle	*dongles;

	dongles = args->dongles;
	if (pthread_mutex_init(&args->begin_mtx, NULL))
		return (1);
	args->mtx_init += 1;
	if (pthread_mutex_init(&args->printer, NULL))
		return (1);
	args->mtx_init += 1;
	i = 0;
	while (i < args->data[0])
	{
		if (pthread_mutex_init(&dongles[i].lock, NULL))
			return (destroy_mutex(args));
		args->mtx_init += 1;
		i++;
	}
	return (0);
}

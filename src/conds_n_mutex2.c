/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conds_n_mutex2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 14:03:43 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/29 14:03:52 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

static int	destroy_coders_mutex(t_args *args)
{
	int			i;
	int			init_mutex;
	t_coder		*coders;

	init_mutex = args->mtx_init;
	coders = args->coders;
	i = 0;
	while (i < (init_mutex - 2 - args->data[0]))
	{
		if (pthread_mutex_destroy(&coders[i].seal))
			return (-1);
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
	if (init_mutex > 0)
		if (pthread_mutex_destroy(&args->begin_mtx))
			return (-1);
	if (init_mutex > 1)
		if (pthread_mutex_destroy(&args->printer))
			return (-1);
	i = 0;
	while (i < init_mutex - 2)
	{
		if (pthread_mutex_destroy(&dongles[i].lock))
			return (-1);
		i++;
	}
	if (destroy_coders_mutex(args))
		return (-1);
	return (0);
}

static int	init_coders_mutex(t_args *args)
{
	int			i;
	t_coder		*coders;

	coders = args->coders;
	i = 0;
	while (i < args->data[0])
	{
		if (pthread_mutex_init(&coders[i].seal, NULL))
			return (1);
		args->mtx_init += 1;
		i++;
	}
	return(0);
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
	if (init_coders_mutex(args))
		return (destroy_mutex(args));
	return (0);
}

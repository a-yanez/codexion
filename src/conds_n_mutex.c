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
	if (init_cond > 0)
		if (pthread_cond_destroy(&args->begin_cnd))
			return (-1);
	i = 0;
	while (i < init_cond - 1)
	{
		if (pthread_cond_destroy(&dongles[i].cond))
			return (-1);
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

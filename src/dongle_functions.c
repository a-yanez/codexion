/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 17:41:33 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/13 17:41:41 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

void	edf(t_dongle *dongle)
{
	t_coder			*tmp;
	t_coder			*coder_a;
	t_coder			*coder_b;
	struct timeval	time_store;

	coder_a = dongle->queue[0];
	coder_b = dongle->queue[1];
	if (coder_b != NULL)
	{
		if (coder_a->last_compile_start > coder_b->last_compile_start)
			ft_pswap((void **)&coder_a, (void **)&coder_b);
	}
}

void	queue(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (dongle->queue[i] == NULL)
		{
			dongle->queue[i] = coder;
			break;
		}
		i++;
	}
	if (dongle->edf)
		edf(dongle);
}

void	pop(t_dongle *dongle)
{
	t_coder	*tmp;

	tmp = dongle->queue[1];
	dongle->queue[0] = tmp;
	dongle->queue[1] = NULL;
}

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
#include <stdio.h>

/*
void	edf(t_dongle *dongle)
{
	t_coder			*codr_a;
	t_coder			*codr_b;

	codr_a = dongle->queue[0];
	codr_b = dongle->queue[1];
	if (codr_b != NULL)
	{
		if (t_diff(codr_a->last_compile_start, codr_b->last_compile_start) > 0)
			ft_pswap((void **)&codr_a, (void **)&codr_b);
	}
}
*/

int	edf(t_dongle *dongle)
{
	t_coder	*c1;
	t_coder	*c2;
	t_tmval	comp1;
	t_tmval	comp2;

	c1 = dongle->queue[0];
	c2 = dongle->queue[1];
	if (c2 != NULL)
	{
		if (safe_mutex_lock(&c1->seal))
			return (1);
		comp1 = c1->last_compile_start;
		if (safe_mutex_unlock(&c1->seal, 0))
			return (1);
		if (safe_mutex_lock(&c2->seal))
			return (1);
		comp2 = c2->last_compile_start;
		if (safe_mutex_unlock(&c2->seal, 0))
			return (1);
		if (t_diff(comp1, comp2) > 0)
			ft_pswap((void **)&c1, (void **)&c2);
	}
	return (0);
}

/*
void	queue(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (dongle->queue[i] == NULL)
		{
			dongle->queue[i] = coder;
			break ;
		}
		i++;
	}
	if (dongle->edf)
		edf(dongle);
}
 */

int	queue(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (dongle->queue[i] == NULL)
		{
			dongle->queue[i] = coder;
			break ;
		}
		i++;
	}
	if (dongle->edf)
	{
		if (edf(dongle))
			return (1);
	}
	return (0);
}

void	pop(t_dongle *dongle)
{
	t_coder	*tmp;

	tmp = dongle->queue[1];
	dongle->queue[0] = tmp;
	dongle->queue[1] = NULL;
}

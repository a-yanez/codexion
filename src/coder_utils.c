/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 17:01:33 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/21 17:01:44 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <unistd.h>
#include <stdio.h>

int	wait(pthread_mutex_t *m, pthread_cond_t *c, int *ready, int *num)
{
	if (safe_mutex_lock(m))
		return (1);
	*ready += 1;
	if (*ready < *num + 1)
	{
		printf("waiting...\n");
		if (safe_cond_wait(c, m))
			return (safe_mutex_unlock(m));
	}
	else
	{
		*ready = 0;
		if (safe_cond_broadcast(c))
			return (safe_mutex_unlock(m));
	}
	return (safe_mutex_unlock(m));
}

int	print_take_dongle(t_coder *coder, volatile struct timeval *t)
{
	if (*(coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(coder->printer))
		return (1);
	if (*(coder->poison) != 0)
		return (safe_mutex_unlock(coder->printer));
	printf("%ld %d has taken a dongle\n", t_diff(*t, *coder->ref), coder->n_id);
	return (safe_mutex_unlock(coder->printer));
}

int	print_action(t_coder *cod, char *act, volatile struct timeval *t)
{
	if (*(cod->poison) != 0)
		return (1);
	if (safe_mutex_lock(cod->printer))
		return (1);
	if (*(cod->poison) != 0)
		return (safe_mutex_unlock(cod->printer));
	printf("%ld %d is %s\n", t_diff(*t, *cod->ref), cod->n_id, act);
	return (safe_mutex_unlock(cod->printer));
}

int	take_dng(t_coder *cod, t_dongle *dng, volatile struct timeval *t)
{
	if (*(cod->poison) != 0)
		return (1);
	while (dng == NULL)
	{
		if (*(cod->poison) != 0)
			return (1);
	}
	if (safe_mutex_lock(&dng->lock))
		return (1);
	queue(dng, cod);
	while (dng->on_use || dng->queue[0]->n_id != cod->n_id)
	{
		if (safe_cond_wait(&dng->cond, &dng->lock))
			return (safe_mutex_unlock(&dng->lock));
	}
	while (t_diff(*t, dng->last_used) < dng->cool_down)
	{
		if (s_tmwt(&dng->cond, &dng->lock, &dng->ts))
			return (safe_mutex_unlock(&dng->lock));
	}
	dng->on_use = 1;
	pop(dng);
	return (safe_mutex_unlock(&dng->lock));
}

int	release_dongle(t_dongle *dongle, t_coder *coder)
{
	if (*(coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	if (safe_gettimeofday(&dongle->last_used))
		return (safe_mutex_unlock(&dongle->lock));
	if (set_timeout(&dongle->ts, dongle->cool_down))
		return (safe_mutex_unlock(&dongle->lock));
	dongle->on_use = 0;
	if (safe_cond_signal(&dongle->cond))
		return (safe_mutex_unlock(&dongle->lock));
	return (safe_mutex_unlock(&dongle->lock));
}

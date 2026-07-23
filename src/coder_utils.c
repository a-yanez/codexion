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

int	barrier_wait(t_c_args *c_args)
{
	if (safe_mutex_lock(c_args->begin_mtx))
		return (1);
	*c_args->coder_ready += 1;
	if (*c_args->coder_ready < *c_args->coder_num + 1)
	{
		printf("waiting...\n");
		if (safe_cond_wait(c_args->begin_cnd, c_args->begin_mtx))
			return (1);
	}
	else
	{
		*c_args->coder_ready = 0;
		if (safe_cond_broadcast(c_args->begin_cnd))
			return (1);
	}
	return (safe_mutex_unlock(c_args->begin_mtx));
}

int	print_take_dongle(t_coder *coder, volatile struct timeval *t)
{
	if (*(coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(coder->printer))
		return (1);
	printf("%ld %d has taken a dongle\n", t_diff(*t, *coder->ref), coder->n_id);
	return (safe_mutex_unlock(coder->printer));
}

int	print_action(t_coder *coder, char *action, volatile struct timeval *t)
{
	if (*(coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(coder->printer))
		return (1);
	printf("%ld %d is %s\n", t_diff(*t, *coder->ref), coder->n_id, action);
	return (safe_mutex_unlock(coder->printer));
}

int	take_dongle(t_coder *coder, t_dongle *dongle, volatile struct timeval *t)
{
	if (*(coder->poison) != 0)
		return (1);
	while(dongle == NULL)
	{
		if (*(coder->poison) != 0)
			return (1);
	}
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	queue(dongle, coder);
	while (dongle->on_use || dongle->queue[0]->n_id != coder->n_id)
	{
		if (safe_cond_wait(&dongle->cond, &dongle->lock))
			return (1);
	}
	while (t_diff(*t, dongle->last_used) < dongle->cool_down)
	{
		if (s_tmwt(&dongle->cond, &dongle->lock, &dongle->ts))
			return (1);
	}
	dongle->on_use = 1;
	pop(dongle);
	return (safe_mutex_unlock(&dongle->lock));
}

int	release_dongle(t_dongle *dongle)
{
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	if (safe_gettimeofday(&dongle->last_used))
		return (1);
	if (set_timeout(&dongle->ts, dongle->cool_down))
		return (1);
	dongle->on_use = 0;
	if (safe_cond_signal(&dongle->cond))
		return (1);
	if (safe_mutex_unlock(&dongle->lock))
		return (1);
	return (safe_mutex_unlock(&dongle->lock));
}

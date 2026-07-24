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
			return (safe_mutex_unlock(c_args->begin_mtx));
	}
	else
	{
		*c_args->coder_ready = 0;
		if (safe_cond_broadcast(c_args->begin_cnd))
			return (safe_mutex_unlock(c_args->begin_mtx));
	}
	return (safe_mutex_unlock(c_args->begin_mtx));
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

int	print_action(t_coder *coder, char *action, volatile struct timeval *t)
{
	if (*(coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(coder->printer))
		return (1);
	if (*(coder->poison) != 0)
		return (safe_mutex_unlock(coder->printer));
	printf("%ld %d is %s\n", t_diff(*t, *coder->ref), coder->n_id, action);
	return (safe_mutex_unlock(coder->printer));
}

int	take_dongle(t_coder *codr, t_dongle *dongl, volatile struct timeval *t)
{
	if (*(codr->poison) != 0)
		return (1);
	while (dongl == NULL)
	{
		if (*(codr->poison) != 0)
			return (1);
	}
	if (safe_mutex_lock(&dongl->lock))
		return (1);
	queue(dongl, codr);
	while (dongl->on_use || dongl->queue[0]->n_id != codr->n_id)
	{
		if (safe_cond_wait(&dongl->cond, &dongl->lock))
			return (safe_mutex_unlock(&dongl->lock));
	}
	while (t_diff(*t, dongl->last_used) < dongl->cool_down)
	{
		if (s_tmwt(&dongl->cond, &dongl->lock, &dongl->ts))
			return (safe_mutex_unlock(&dongl->lock));
	}
	dongl->on_use = 1;
	pop(dongl);
	return (safe_mutex_unlock(&dongl->lock));
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

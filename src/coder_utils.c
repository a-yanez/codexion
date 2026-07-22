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
#include <stdio.h>

int	barrier_wait(t_c_args *c_args)
{
	int	signal;

	signal = safe_mutex_lock(c_args->begin_mtx);
	if (signal)
		return (signal);
	*c_args->coder_ready += 1;
	if (*c_args->coder_ready < *c_args->coder_num + 1)
	{
		printf("waiting...\n");
		signal	= safe_cond_wait(c_args->begin_cnd, c_args->begin_mtx);
		if (signal)
			return (signal);
	}
	else
	{
		*c_args->coder_ready = 0;
		signal	= safe_cond_broadcast(c_args->begin_cnd);
		if (signal)
			return (signal);
	}
	signal = safe_mutex_unlock(c_args->begin_mtx);
	return (signal);
}

int	print_take_dongle(t_coder *coder, volatile struct timeval *t)
{
	int	signal;

	if (*(coder->poison) != 0)
		return (1);
	signal = safe_mutex_lock(coder->printer);
	if (signal)
		return (signal);
	printf("%ld %d has taken a dongle\n", t_diff(*t, *coder->ref), coder->n_id);
	signal = safe_mutex_unlock(coder->printer);
	return (signal);
}

int	print_action(t_coder *coder, char *action, volatile struct timeval *t)
{
	int	signal;

	if (*(coder->poison) != 0)
		return (1);
	signal = safe_mutex_lock(coder->printer);
	if (signal)
		return (signal);
	printf("%ld %d is %s\n", t_diff(*t, *coder->ref), coder->n_id, action);
	signal = safe_mutex_unlock(coder->printer);
	return (signal);
}

int	take_dongle(t_coder *coder, t_dongle *dongle, volatile struct timeval *t)
{
	int	signal;

	if (!dongle)
		return (1);
	signal = safe_mutex_lock(&dongle->lock);
	if (signal)
		return (signal);
	queue(dongle, coder);
	while (dongle->on_use || dongle->queue[0]->n_id != coder->n_id)
	{
		printf("%d will wait...\n", coder->n_id);
		signal	= safe_cond_wait(&dongle->cond, &dongle->lock);
		if (signal)
			return (signal);
	}
	while (t_diff(*t, dongle->last_used) < dongle->cool_down)
	{
		printf("%d will wait timely...\n", coder->n_id);
		signal	= s_tmwt(&dongle->cond, &dongle->lock, &dongle->ts);
		if (signal)
			return (signal);
	}
	dongle->on_use = 1;
	pop(dongle);
	signal = safe_mutex_unlock(&dongle->lock);
	return (signal);
}

int	release_dongle(t_dongle *dongle)
{
	int	signal;

	signal = safe_mutex_lock(&dongle->lock);
	if (signal)
		return (signal);
	signal = safe_gettimeofday(&dongle->last_used);
	if (signal)
		return (signal);
	signal = set_timeout(&dongle->ts, dongle->cool_down);
	if (signal)
		return (signal);
	dongle->on_use = 0;
	signal = safe_cond_signal(&dongle->cond);
	if (signal)
		return (signal);
	signal = safe_mutex_unlock(&dongle->lock);
	if (signal)
		return (signal);
	signal = safe_mutex_unlock(&dongle->lock);
	return (signal);
}

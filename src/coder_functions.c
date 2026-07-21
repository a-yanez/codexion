/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 13:54:23 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/20 14:47:07 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static int	take_dongle(t_coder *coder, t_dongle *dongle, volatile struct timeval *t)
{
	int	signal;

	signal = safe_mutex_lock(&dongle->lock);
	if (signal)
		return (signal);
	queue(dongle, coder);
	while (dongle->on_use || dongle->queue[0]->n_id != coder->n_id)
	{
		signal	= safe_cond_wait(&dongle->cond, &dongle->lock);
		if (signal)
			return (signal);
	}
	while (t_diff(*t, dongle->last_used) < dongle->cool_down)
	{
		signal	= s_tmwt(&dongle->cond, &dongle->lock, &dongle->ts);
		if (signal)
			return (signal);
	}
	dongle->on_use = 1;
	pop(dongle);
	signal = safe_mutex_unlock(&dongle->lock);
	return (signal);
}

static int	release_dongle(t_dongle *dongle)
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

static int coder_loop_one(t_coder *coder, struct timeval *t)
{
	int	signal;

	signal = take_dongle(coder, coder->dongles[0], t);
	if (signal)
		return (signal);
	signal = print_take_dongle(coder, t);
	if (signal)
		return (signal);
	take_dongle(coder, coder->dongles[1], t);
	if (signal)
		return (signal);
	signal = print_action(coder, "compiling", t);
	if (signal)
		return (signal);
	signal = safe_gettimeofday(&coder->last_compile_start);
	if (signal)
		return (signal);
	usleep(coder->compt_time);
	return (signal);
}

static int coder_loop_two(t_coder *coder, struct timeval *t)
{
	int	signal;

	signal = release_dongle(coder->dongles[0]);
	if (signal)
		return (signal);
	signal = release_dongle(coder->dongles[1]);
	if (signal)
		return (signal);
	signal = print_action(coder, "debugging", t);
	if (signal)
		return (signal);
	usleep(coder->db_time);
	signal = print_action(coder, "refactoring", t);
	if (signal)
		return (signal);
	usleep(coder->refac_time);
	coder->comp_times += 1;
	return (signal);
}

void	*coder_rutine(void *args)
{
	t_coder			*coder;
	struct timeval	*t;
	int 			signal;

	signal = barrier_wait((t_c_args *)args);
	if (signal)
		return (NULL);
	coder = ((t_c_args *)args)->coder;
	t = ((t_c_args *)args)->t;
	while (coder->comp_times <= coder->cycles && !signal)
	{
		signal = coder_loop_one(coder, t);
		if (signal)
			break ;
		signal = coder_loop_two(coder, t);
		if (signal)
			break ;
	}
	signal = safe_mutex_lock(((t_c_args *)args)->begin_mtx);
	if (signal)
		return (NULL);
	*((t_c_args *)args)->coder_ready += 1;
	signal = safe_mutex_unlock(((t_c_args *)args)->begin_mtx);
	if (signal)
		return (NULL); //HERE SOMETHING ELSE!
	return (NULL);
}

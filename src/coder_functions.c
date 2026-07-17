/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 13:54:23 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/07 13:54:29 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

void	barrier_wait(t_c_args *c_args)
{
	pthread_mutex_lock(c_args->begin_mtx);
	*c_args->coder_ready += 1;
	if (*c_args->coder_ready < *c_args->coder_num + 1)
		pthread_cond_wait(c_args->begin_cnd, c_args->begin_mtx);
	else
	{
		*c_args->coder_ready = 0;
		pthread_cond_broadcast(c_args->begin_cnd);
	}
	pthread_mutex_unlock(c_args->begin_mtx);
}

static void	take_dongle(t_coder *coder, t_dongle *dongle, struct timeval *t)
{
	pthread_mutex_lock(&dongle->lock);
	queue(dongle, coder);
	while (dongle->on_use || &dongle->queue[0] != &coder)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	while (t_diff(*t, dongle->last_used) < dongle->cool_down)
		pthread_cond_timedwait(&dongle->cond, &dongle->lock, &dongle->ts);
	dongle->on_use = 1;
	pop(dongle);
	pthread_mutex_unlock(&dongle->lock);
	pthread_mutex_lock(coder->printer);
	printf("%ld %d has taken a dongle\n", t_diff(*t, *coder->ref), coder->n_id);
	pthread_mutex_unlock(coder->printer);
}

static void	release_dongle(t_coder *coder, t_dongle *dongle, struct timeval *t)
{
	pthread_mutex_lock(&dongle->lock);
	gettimeofday(&dongle->last_used, NULL);
	set_timeout(&dongle->ts, dongle->cool_down);
	dongle->on_use = 0;
	pthread_cond_signal(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

static void	print_action(t_coder *coder, char *action, struct timeval *t)
{
	pthread_mutex_lock(coder->printer);
	printf("%ld %d is %s\n", t_diff(*t, *coder->ref), coder->n_id, action);
	pthread_mutex_unlock(coder->printer);
}

void	*coder_rutine(void *args)
{
	t_coder			*coder;
	struct timeval	*t;

	barrier_wait((t_c_args *)args);
	coder = ((t_c_args *)args)->coder;
	t = ((t_c_args *)args)->t;
	take_dongle(coder, coder->dongles[0], t);
	take_dongle(coder, coder->dongles[1], t);
	print_action(coder, "compiling", t);
	usleep(coder->compt_time);
	release_dongle(coder, coder->dongles[0], t);
	release_dongle(coder, coder->dongles[1], t);
	print_action(coder, "debugging", t);
	usleep(coder->db_time);
	print_action(coder, "refactoring", t);
	usleep(coder->refac_time);
	return (NULL);
}

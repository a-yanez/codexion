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
#include <bits/types/struct_timeval.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval	t_measure;
	suseconds_t		t;

	pthread_mutex_lock(&dongle->lock);
	queue(dongle, coder);
	while (dongle->on_use && &dongle->queue[0] != &coder)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	dongle->on_use = 1;
	gettimeofday(&t_measure, NULL);
	t = t_measure.tv_usec / 1000;
	pop(dongle);
	pthread_mutex_unlock(&dongle->lock);
	pthread_mutex_lock(coder->printer);
	printf("%ld %d has taken a dongle\n", t, coder->n_id);
	pthread_mutex_unlock(coder->printer);
}

int	release_dongle(t_coder *coder, t_dongle *left, t_dongle *right)
{
	struct timeval	time_measure;

	if (gettimeofday(&time_measure, NULL) < 0)
		return (-1);
	pthread_mutex_lock(&left->lock);
	left->last_used = time_measure.tv_usec;
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	right->last_used = time_measure.tv_usec;
	pthread_mutex_unlock(&right->lock);
	return (1);
}

void	print_action(t_coder *coder, char *action)
{
	struct timeval	t_measure;
	suseconds_t		t;

	pthread_mutex_lock(coder->printer);
	gettimeofday(&t_measure, NULL);
	t = t_measure.tv_usec / 1000;
	printf("%ld %d is %s\n", t, coder->n_id, action);
	pthread_mutex_unlock(coder->printer);
}

void	*coder_rutine(void *args)
{
	t_coder		*coder;

	coder = (t_coder *)args;
	take_dongle(coder, coder->dongles[0]);
	take_dongle(coder, coder->dongles[1]);
	print_action(coder, "compiling");
	usleep(coder->compt_time);
	print_action(coder, "debugging");
	usleep(coder->db_time);
	print_action(coder, "refactoring");
	usleep(coder->refac_time);
	return (NULL);
}

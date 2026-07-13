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

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval time_measure;

	pthread_mutex_lock(&dongle->lock);
	queue(dongle, coder);
	while (!dongle->avail && &dongle->queue[0] != &coder)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	dongle->avail = 0;
	pop(dongle);
	pthread_mutex_unlock(&dongle->lock);
}

void	take_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	struct timeval	time_measure;

	pthread_mutex_lock(&left->lock);
	queue(left, coder);
	while (!left->avail && &left->queue[0] != &coder)
		pthread_cond_wait(&left->cond, &left->lock);
	left->avail = 0;
	pop(left);
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	queue(right, coder);
	while (!right->avail && left->queue[0] != coder)
		pthread_cond_wait(&right->cond, &right->lock);
	right->avail = 0;
	pop(right);
	pthread_mutex_unlock(&right->lock);
}

int	release_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	struct timeval	time_measure;

	if (gettimeofday(&time_measure, NULL) < 0)
		return (0);
	pthread_mutex_lock(&left->lock);
	left->last_used = time_measure.tv_usec;
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	right->last_used = time_measure.tv_usec;
	pthread_mutex_unlock(&right->lock);
	return (1);
}

void	compile(t_coder *coder)
{
	printf("%d %d is compiling");
	// THIS NEEDS A MUTEX FOR PRINTING!!
}

void	*coder_rutine(void *args)
{
	t_coder		*coder;
	t_dongle	*left;
	t_dongle	*right;

	coder = (t_coder *)args;
	left = coder->dongle_left;
	right = coder->dongle_right;
	pthread_mutex_lock(&left->lock);
	if (!left->avail)
	{
		pthread_cond_wait(&left->cond, &left->lock);
	}
	left->avail = 0;
	if (!right->avail)
	{
		pthread_cond_wait(&right->cond, &right->lock);
	}
	right->avail = 0;
	return (NULL);
}

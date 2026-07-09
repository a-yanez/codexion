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
#include <pthread.h>
#include <time.h>

void	take_dongles(t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->lock);
	while (!left->avail)
	{
		pthread_cond_wait(&left->cond, &left->lock);
	}
	left->avail = 0;
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	while (!right->avail)
	{
		pthread_cond_wait(&right->cond, &right->lock);
	}
	right->avail = 0;
	pthread_mutex_unlock(&right->lock);
}

// COMPLETE THIS PART!!!!!!! FIND OUT HOW TO SIGNAL THE RELEASE!
void	release_dongles(t_dongle *left, t_dongle *right)
{
		pthread_mutex_lock()
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

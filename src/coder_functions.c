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
#include <pthread.h>
#include <sys/time.h>

int	check_avail(t_dongle *dongle)
{
	suseconds_t last_used;

	last_used = dongle->last_used.tv_usec;
	gettimeofday(&dongle->last_used, NULL);
	if (dongle->last_used.tv_usec - last_used > dongle->cool_down)
	{
		dongle->avail = 1;
	}
	return (1);
}

//EDF GOES HERE!
void	queue(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (dongle->queue[i] == NULL)
		{
			dongle->queue[i] = coder;
			break;
		}
		i++;
	}
}

void edf(t_dongle *dongle)
{
	t_coder			*tmp;
	struct timeval	time_store;

	if (dongle->queue[1] != NULL)
	{
		\\Check the time!
	}
}

void	pop(t_dongle *dongle)
{
	t_coder	*tmp;

	tmp = dongle->queue[1];
	dongle->queue[0] = tmp;
	dongle->queue[1] = NULL;
}

void	take_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->lock);
	check_avail(left);
	queue(left, coder->n_id);
	while (!left->avail && &left->queue[0] != &coder)
	{
		pthread_cond_wait(&left->cond, &left->lock);
	}
	left->avail = 0;
	pop(left);
	gettimeofday(&left->last_used, NULL);
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	queue(right, coder);
	while (!right->avail && left->queue[0] != coder)
	{
		pthread_cond_wait(&right->cond, &right->lock);
	}
	right->avail = 0;
	pop(right);
	gettimeofday(&right->last_used, NULL);
	pthread_mutex_unlock(&right->lock);
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

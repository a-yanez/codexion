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
#include <sys/time.h>

int	check_avail(t_dongle *dongle)
{
	suseconds_t last_used;

	last_used = dongle->time.tv_usec;
	gettimeofday(&dongle->time, NULL);
	if (dongle->time.tv_usec - last_used > dongle->cool_down)
	{
		dongle->avail = 1;
	}
	return (1);
}

//EDF GOES HERE!
void	queue(t_dongle *dongle, int id)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (dongle->queue[i] == 0)
		{
			dongle->queue[i] = id;
			break;
		}
		i++;
	}
}

void	pop(t_dongle *dongle)
{
	int	tmp;

	tmp = dongle->queue[1];
	dongle->queue[0] = tmp;
	dongle->queue[1] = 0;
}

void	take_dongles(int id, t_dongle *left, t_dongle *right)
{
	pthread_mutex_lock(&left->lock);
	check_avail(left);
	queue(left, id);
	while (!left->avail && left->queue[0] != id)
	{
		pthread_cond_wait(&left->cond, &left->lock);
	}
	left->avail = 0;
	pop(left);
	gettimeofday(&left->time, NULL);
	pthread_mutex_unlock(&left->lock);
	pthread_mutex_lock(&right->lock);
	queue(right, id);
	while (!right->avail && left->queue[0] != id)
	{
		pthread_cond_wait(&right->cond, &right->lock);
	}
	right->avail = 0;
	pop(right);
	gettimeofday(&right->time, NULL);
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

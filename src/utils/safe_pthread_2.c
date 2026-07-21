/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pthread_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 14:50:19 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/21 14:50:29 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <stdio.h>

int	safe_mutex_destroy(pthread_mutex_t *mutex)
{
	int	signal;

	signal = pthread_mutex_destroy(mutex);
	if (signal)
	{
		fprintf(stderr, "Mutex destroying failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_cond_destroy(pthread_cond_t *cond)
{
	int	signal;

	signal = pthread_cond_destroy(cond);
	if (signal)
	{
		fprintf(stderr, "Cond destroying failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_mutex_lock(pthread_mutex_t *mutex)
{
	int	signal;

	signal = pthread_mutex_lock(mutex);
	if (signal)
	{
		fprintf(stderr, "Mutex locking failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_mutex_unlock(pthread_mutex_t *mutex)
{
	int	signal;

	signal = pthread_mutex_unlock(mutex);
	if (signal)
	{
		fprintf(stderr, "Mutex unlocking failed\n");
		return (signal);
	}
	return (signal);
}

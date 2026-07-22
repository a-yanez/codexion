/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pthread_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 14:33:02 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/21 14:33:04 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
//#include <stdio.h>

int	safe_thread_create(pthread_t t_id, void *(*routine)(void *), void *arg)
{
	int	signal;

	signal = pthread_create(&t_id, NULL, routine, arg);
	if (signal)
	{
		//fprintf(stderr, "Thread creation failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_thread_join(pthread_t thread_id)
{
	int	signal;

	signal = pthread_join(thread_id, NULL);
	if (signal)
	{
		//fprintf(stderr, "Thread joining failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_mutex_init(pthread_mutex_t mutex)
{
	int	signal;

	signal = pthread_mutex_init(&mutex, NULL);
	if (signal)
	{
		//fprintf(stderr, "Mutex locking failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_cond_init(pthread_cond_t cond)
{
	int	signal;

	signal = pthread_cond_init(&cond, NULL);
	if (signal)
	{
		//fprintf(stderr, "Mutex locking failed\n");
		return (signal);
	}
	return (signal);
}

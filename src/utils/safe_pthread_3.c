/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pthread_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 15:24:23 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/21 15:24:26 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"
#include <asm-generic/errno.h>
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <asm-generic/errno.h>
#include <stdio.h>

int	safe_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	int	signal;

	signal = pthread_cond_wait(cond, mutex);
	if (signal)
	{
		fprintf(stderr, "Thread conditional wait failed\n");
		return (signal);
	}
	return (signal);
}

int	s_tmwt(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *t)
{
	int	signal;

	signal = pthread_cond_timedwait(c, m, t);
	if (signal != 0 && signal != ETIMEDOUT)
	{
		fprintf(stderr, "Thread timed wait failed. Code: %d\n", signal);
		return (signal);
	}
	return (0);
}

int	safe_cond_signal(pthread_cond_t *cond)
{
	int	signal;

	signal = pthread_cond_signal(cond);
	if (signal)
	{
		fprintf(stderr, "Conditional signaling failed\n");
		return (signal);
	}
	return (signal);
}

int	safe_cond_broadcast(pthread_cond_t *cond)
{
	int	signal;

	signal = pthread_cond_broadcast(cond);
	if (signal)
	{
		fprintf(stderr, "Conditional broadcasting failed\n");
		return (signal);
	}
	return (signal);
}

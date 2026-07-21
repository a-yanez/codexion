/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:46:15 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/15 14:46:21 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

int	safe_gettimeofday(struct timeval *t)
{
	int	signal;

	signal = gettimeofday(t, NULL);
	if (signal)
	{
		fprintf(stderr, "Time measurement failed\n");
		return (signal);
	}
	return (signal);
}

static struct timespec	timeval_to_timespec(struct timeval tv)
{
	struct timespec	ts;

	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000;
	}
	return (ts);
}

static void	add_ms_to_timespec(struct timespec *ts, int ms)
{
	ts->tv_sec += (ms / 1000);
	ts->tv_nsec += (ms % 1000) * 1000000;
	if (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec += 1;
		ts->tv_nsec -= 1000000000;
	}
}

int	set_timeout(struct timespec *ts, int timeout_ms)
{
	int				signal;
	struct timeval	tv;

	signal = safe_gettimeofday(&tv);
	if (signal)
		return (signal);
	*ts = timeval_to_timespec(tv);
	add_ms_to_timespec(ts, timeout_ms);
	return (signal);
}

long	t_diff(struct timeval tv1, struct timeval tv2)
{
	long	diff_s;
	long	diff_usec;
	long	diff_ms;

	diff_s = tv1.tv_sec - tv2.tv_sec;
	diff_usec = tv1.tv_usec - tv2.tv_usec;
	if (diff_usec > 0)
	{
		diff_s -= 1;
		diff_usec += 1000000;
	}
	diff_ms = (diff_s * 1000) + (diff_usec / 1000);
	return (diff_ms);
}

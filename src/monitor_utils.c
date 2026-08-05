/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 18:23:04 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/08/05 15:12:01 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"

int	wait(pthread_mutex_t *m, pthread_cond_t *c, int *ready, int num)
{
	if (safe_mutex_lock(m))
		return (1);
	*ready += 1;
	if (*ready < num + 1)
	{
		while (*ready < num + 1)
		{
			if (safe_cond_wait(c, m))
				return (safe_mutex_unlock(m, 1));
		}
	}
	else
	{
		if (safe_cond_broadcast(c))
			return (safe_mutex_unlock(m, 1));
	}
	if (safe_mutex_unlock(m, 0))
		return (1);
	return (0);
}

int	coders_working(t_args *args)
{
	int	signal;

	signal = 0;
	if (safe_mutex_lock(&args->begin_mtx))
		return (0);
	if (args->coder_done >= 0 && args->coder_done < args->data[0])
		signal = 1;
	if (safe_mutex_unlock(&args->begin_mtx, 0))
		return (0);
	return (signal);
}

int64_t	calculate_delta(t_coder *coder, t_tmval t)
{
	int64_t	t_delta;
	t_tmval	last_comp;

	if (safe_mutex_lock(&coder->seal))
		return (-1);
	last_comp = coder->last_compile_start;
	if (safe_mutex_unlock(&coder->seal, 0))
		return (-1);
	t_delta = t_diff(t, last_comp);
	return (t_delta);
}

int	is_coder_done(t_coder *coder)
{
	int	finished;

	if (safe_mutex_lock(&coder->seal))
		return (-1);
	finished = coder->finished;
	if (safe_mutex_unlock(&coder->seal, 0))
		return (-1);
	return (finished);
}

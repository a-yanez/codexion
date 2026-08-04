/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 18:00:08 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/29 18:00:10 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"

static int	check_and_lock(t_coder *c, t_dongle *d, t_c_args *a)
{
	if (check_poison(a->begin_mtx, c->poison) != 0)
		return (1);
	while (d == NULL)
		return (1);
	if (safe_mutex_lock(&d->lock))
		return (1);
	return (0);
}

static int	wait_for_condition(t_coder *c, t_dongle *d, t_c_args *a)
{
	while (d->on_use)
	{
		if (c->holding == 1 && d->edf)
			return (safe_mutex_unlock(&d->lock, 3));
		if (safe_cond_wait(&d->cond, &d->lock))
			return (safe_mutex_unlock(&d->lock, 1));
		if (check_poison(a->begin_mtx, c->poison))
			return (safe_mutex_unlock(&d->lock, 1));
	}
	return (0);
}

static int	wait_for_cooldown(t_coder *c, t_dongle *d, t_c_args *a)
{
	int64_t	delta_t;

	delta_t = t_diff(c->own, d->last_used);
	while (delta_t < d->cool_down || d->queue[0]->n_id != c->n_id)
	{
		if (d->queue[0]->n_id != c->n_id)
		{
			if (safe_cond_signal(&d->cond))
				return (safe_mutex_unlock(&d->lock, 1));
			if (safe_cond_wait(&d->cond, &d->lock))
				return (safe_mutex_unlock(&d->lock, 1));
		}
		else
		{
			if (s_tmwt(&d->cond, &d->lock, &d->ts))
				return (safe_mutex_unlock(&d->lock, 1));
		}
		if (safe_gettimeofday(&c->own))
			return (safe_mutex_unlock(&d->lock, 1));
		delta_t = t_diff(c->own, d->last_used);
		if (check_poison(a->begin_mtx, c->poison))
			return (safe_mutex_unlock(&d->lock, 1));
	}
	return (0);
}

int	take(t_coder *c, t_dongle *d, t_c_args *a)
{
	int	drop_signal;

	if (check_and_lock(c, d, a))
		return (1);
	drop_signal = wait_for_condition(c, d, a);
	if (drop_signal == 3)
		return (3);
	else if (drop_signal)
		return (1);
	if (queue(d, c))
		return (safe_mutex_unlock(&d->lock, 1));
	if (safe_gettimeofday(&c->own))
		return (safe_mutex_unlock(&d->lock, 1));
	if (wait_for_cooldown(c, d, a))
		return (1);
	d->on_use = 1;
	c->holding += 1;
	pop(d);
	if (safe_mutex_unlock(&d->lock, 0))
		return (1);
	return (0);
}

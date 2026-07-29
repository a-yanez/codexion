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
	while (d->on_use || d->queue[0]->n_id != c->n_id)
	{
		if (safe_cond_wait(&d->cond, &d->lock))
			return (safe_mutex_unlock(&d->lock, 1));
		if (check_poison(a->begin_mtx, c->poison))
			return (safe_mutex_unlock(&d->lock, 1));
	}
	return (0);
}

static int	wait_for_cooldown(t_coder *c, t_dongle *d, t_c_args *a)
{
	if (safe_gettimeofday(&c->own))
		return (safe_mutex_unlock(&d->lock, 1));
	while (t_diff(c->own, d->last_used) < d->cool_down)
	{
		if (s_tmwt(&d->cond, &d->lock, &d->ts))
			return (safe_mutex_unlock(&d->lock, 1));
		if (check_poison(a->begin_mtx, c->poison))
			return (safe_mutex_unlock(&d->lock, 1));
	}
	return (0);
}

int	take(t_coder *c, t_dongle *d, t_c_args *a)
{
	if (check_and_lock(c, d, a))
		return (1);
	if (wait_for_condition(c, d, a))
		return (1);
	if (wait_for_cooldown(c, d, a))
		return (1);
	d->on_use = 1;
	pop(d);
	if (safe_mutex_unlock(&d->lock, 0))
		return (1);
	return (0);
}

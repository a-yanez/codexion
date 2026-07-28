/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 17:01:33 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/21 17:01:44 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/pthreadtypes.h>
#include <bits/types/struct_timeval.h>
#include <unistd.h>
#include <stdio.h>

int	check_poison(pthread_mutex_t *b_mtx, int *poison)
{
	int	signal;

	signal = 0;
	if (safe_mutex_lock(b_mtx))
		return (-1);
	if (*poison == 1)
		signal = 1;
	if (safe_mutex_unlock(b_mtx))
		return (-1);
	return (signal);
}

int	print_take(t_coder *coder, t_c_args *car, volatile t_tmval *t)
{
	if (check_poison(car->begin_mtx, coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(coder->printer))
		return (1);
	if (check_poison(car->begin_mtx, coder->poison) != 0)
		return (safe_mutex_unlock(coder->printer));
	printf("%ld %d has taken a dongle\n", t_diff(*t, *coder->ref), coder->n_id);
	return (safe_mutex_unlock(coder->printer));
}

int	act(t_coder *c, char *ac, t_c_args *ar, volatile t_tmval *t)
{
	if (check_poison(ar->begin_mtx, c->poison) != 0)
		return (1);
	if (safe_mutex_lock(c->printer))
		return (1);
	if (check_poison(ar->begin_mtx, c->poison) != 0)
		return (safe_mutex_unlock(c->printer));
	printf("%ld %d is %s\n", t_diff(*t, *c->ref), c->n_id, ac);
	return (safe_mutex_unlock(c->printer));
}

int	take(t_coder *c, t_dongle *d, t_c_args *a, volatile t_tmval *t)
{
	if (check_poison(a->begin_mtx, c->poison) != 0)
		return (1);
	while (d == NULL)
	{
		if (check_poison(a->begin_mtx, c->poison) != 0)
			return (1);
	}
	if (safe_mutex_lock(&d->lock))
		return (1);
	queue(d, c);
	while (d->on_use || d->queue[0]->n_id != c->n_id)
	{
		if (safe_cond_wait(&d->cond, &d->lock))
			return (safe_mutex_unlock(&d->lock));
		if (check_poison(a->begin_mtx, c->poison))
	        return (safe_mutex_unlock(&d->lock));
	}
	while (t_diff(*t, d->last_used) < d->cool_down)
	{
		if (s_tmwt(&d->cond, &d->lock, &d->ts))
			return (safe_mutex_unlock(&d->lock));
		if (check_poison(a->begin_mtx, c->poison))
	        return (safe_mutex_unlock(&d->lock));
	}
	d->on_use = 1;
	pop(d);
	return (safe_mutex_unlock(&d->lock));
}

int	release(t_dongle *dongle, t_c_args *ar, t_coder *coder)
{
	if (check_poison(ar->begin_mtx, coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	if (safe_gettimeofday(&dongle->last_used))
		return (safe_mutex_unlock(&dongle->lock));
	if (set_timeout(&dongle->ts, dongle->cool_down))
		return (safe_mutex_unlock(&dongle->lock));
	dongle->on_use = 0;
	if (safe_cond_signal(&dongle->cond))
		return (safe_mutex_unlock(&dongle->lock));
	return (safe_mutex_unlock(&dongle->lock));
}

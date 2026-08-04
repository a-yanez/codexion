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
	if (safe_mutex_unlock(b_mtx, 0))
		return (-1);
	return (signal);
}

int	print_take(t_coder *c, t_c_args *ar)
{
	int64_t	delta_t;

	if (safe_mutex_lock(c->printer))
		return (1);
	if (check_poison(ar->begin_mtx, c->poison) != 0)
		return (safe_mutex_unlock(c->printer, 1));
	if (safe_gettimeofday(&c->own))
		return (safe_mutex_unlock(c->printer, 1));
	delta_t = calculate_delta(c, c->own);
	if (delta_t < 0)
		return (safe_mutex_unlock(c->printer, 1));
	else if (delta_t >= ar->burnout)
		return (safe_mutex_unlock(c->printer, 3));
	delta_t = t_diff(c->own, c->ref);
	printf("%lld %d has taken a dongle\n", (long long)delta_t, c->n_id);
	if (safe_mutex_unlock(c->printer, 0))
		return (1);
	return (0);
}

int	act(t_coder *c, char *ac, t_c_args *ar)
{
	int64_t	delta_t;

	if (safe_mutex_lock(c->printer))
		return (1);
	if (check_poison(ar->begin_mtx, c->poison) != 0)
		return (safe_mutex_unlock(c->printer, 1));
	if (safe_gettimeofday(&c->own))
		return (safe_mutex_unlock(c->printer, 1));
	delta_t = calculate_delta(c, c->own);
	if (delta_t < 0)
		return (safe_mutex_unlock(c->printer, 1));
	else if (delta_t >= ar->burnout)
		return (safe_mutex_unlock(c->printer, 3));
	delta_t = t_diff(c->own, c->ref);
	printf("%lld %d is %s\n", (long long)delta_t, c->n_id, ac);
	if (safe_mutex_unlock(c->printer, 0))
		return (1);
	return (0);
}

int	release(t_dongle *dongle, t_c_args *ar, t_coder *coder)
{
	if (check_poison(ar->begin_mtx, coder->poison) != 0)
		return (1);
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	if (safe_gettimeofday(&dongle->last_used))
		return (safe_mutex_unlock(&dongle->lock, 1));
	if (set_timeout(&dongle->ts, dongle->cool_down))
		return (safe_mutex_unlock(&dongle->lock, 1));
	dongle->on_use = 0;
	coder->holding -= 1;
	if (safe_cond_signal(&dongle->cond))
		return (safe_mutex_unlock(&dongle->lock, 1));
	if (safe_mutex_unlock(&dongle->lock, 0))
		return (1);
	return (0);
}

int	drop(t_dongle *dongle, t_coder *coder)
{
	if (safe_mutex_lock(&dongle->lock))
		return (1);
	dongle->on_use = 0;
	coder->holding -= 1;
	if (safe_cond_signal(&dongle->cond))
		return (safe_mutex_unlock(&dongle->lock, 1));
	if (safe_mutex_unlock(&dongle->lock, 0))
		return (1);
	return (0);
}

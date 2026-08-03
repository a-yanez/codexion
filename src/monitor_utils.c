/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 18:23:04 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/29 18:23:06 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"

int	coders_working(t_args *args)
{
	return (args->coder_done >= 0 && args->coder_done < args->data[0]);
}

long	calculate_delta(t_coder *coder, t_tmval t)
{
	long	t_delta;
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

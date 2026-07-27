/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 13:54:23 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/20 14:47:07 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"
#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

static int	coder_loop_one(t_coder *coder, t_c_args *ar, struct timeval *t)
{
	if (take(coder, coder->dongles[0], ar, t))
		return (1);
	if (print_take(coder, ar, t))
		return (1);
	if (take(coder, coder->dongles[1], ar, t))
		return (1);
	if (print_take(coder, ar, t))
		return (1);
	if (act(coder, "compiling", ar, t))
		return (1);
	if (safe_gettimeofday(&coder->last_compile_start))
		return (1);
	usleep(coder->compt_time);
	return (0);
}

static int	coder_loop_two(t_coder *coder, t_c_args *ar, struct timeval *t)
{
	if (release(coder->dongles[0], ar, coder))
		return (1);
	if (release(coder->dongles[1], ar, coder))
		return (1);
	if (act(coder, "debugging", ar, t))
		return (1);
	usleep(coder->db_time);
	if (act(coder, "refactoring", ar, t))
		return (1);
	usleep(coder->refac_time);
	coder->comp_times += 1;
	return (0);
}

static int	final_part(t_c_args *c_args, t_coder *coder)
{
	if (*(coder->poison))
		return (1);
	if (safe_mutex_lock(c_args->begin_mtx))
		return (1);
	*c_args->coder_done += 1;
	return (safe_mutex_unlock(c_args->begin_mtx));
}

void	*coder_routine(void *args)
{
	t_coder			*coder;
	t_c_args		*ar;
	struct timeval	*t;
	int				signal;

	ar = (t_c_args *)args;
	signal = wait(ar->begin_mtx, ar->begin_cnd, ar->coder_ready, ar->coder_num);
	if (signal)
		return ((void *)(intptr_t)signal);
	coder = ar->coder;
	t = ar->t;
	while (coder->comp_times < coder->cycles && !(*(coder->poison)))
	{
		signal = coder_loop_one(coder, ar, t);
		if (signal)
			return ((void *)(intptr_t)signal);
		signal = coder_loop_two(coder, ar, t);
		if (signal)
			return ((void *)(intptr_t)signal);
	}
	final_part((t_c_args *)args, coder);
	return (NULL);
}

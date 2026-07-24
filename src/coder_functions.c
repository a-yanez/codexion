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

static int	coder_loop_one(t_coder *coder, struct timeval *t)
{
	if (take_dongle(coder, coder->dongles[0], t))
		return (1);
	if (print_take_dongle(coder, t))
		return (1);
	if (take_dongle(coder, coder->dongles[1], t))
		return (1);
	if (print_take_dongle(coder, t))
		return (1);
	if (print_action(coder, "compiling", t))
		return (1);
	if (safe_gettimeofday(&coder->last_compile_start))
		return (1);
	usleep(coder->compt_time);
	return (0);
}

static int	coder_loop_two(t_coder *coder, struct timeval *t)
{
	if (release_dongle(coder->dongles[0], coder))
		return (1);
	if (release_dongle(coder->dongles[1], coder))
		return (1);
	if (print_action(coder, "debugging", t))
		return (1);
	usleep(coder->db_time);
	if (print_action(coder, "refactoring", t))
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
	*c_args->coder_ready += 1;
	return (safe_mutex_unlock(c_args->begin_mtx));
}

void	*coder_rutine(void *args)
{
	t_coder			*coder;
	struct timeval	*t;
	int				signal;

	signal = barrier_wait((t_c_args *)args);
	if (signal)
		return ((void *)(intptr_t)signal);
	coder = ((t_c_args *)args)->coder;
	t = ((t_c_args *)args)->t;
	while (coder->comp_times < coder->cycles && !(*(coder->poison)))
	{
		signal = coder_loop_one(coder, t);
		if (signal)
			return ((void *)(intptr_t)signal);
		signal = coder_loop_two(coder, t);
		if (signal)
			return ((void *)(intptr_t)signal);
	}
	final_part((t_c_args *)args, coder);
	return (NULL);
}

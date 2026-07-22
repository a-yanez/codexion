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
#include <unistd.h>

static int coder_loop_one(t_coder *coder, struct timeval *t)
{
	int	signal;

	signal = take_dongle(coder, coder->dongles[0], t);
	if (signal)
		return (signal);
	signal = print_take_dongle(coder, t);
	if (signal)
		return (signal);
	signal = take_dongle(coder, coder->dongles[1], t);
	if (signal)
		return (signal);
	signal = print_take_dongle(coder, t);
	if (signal)
		return (signal);
	signal = print_action(coder, "compiling", t);
	if (signal)
		return (signal);
	signal = safe_gettimeofday(&coder->last_compile_start);
	if (signal)
		return (signal);
	usleep(coder->compt_time);
	return (signal);
}

static int coder_loop_two(t_coder *coder, struct timeval *t)
{
	int	signal;

	signal = release_dongle(coder->dongles[0]);
	if (signal)
		return (signal);
	signal = release_dongle(coder->dongles[1]);
	if (signal)
		return (signal);
	signal = print_action(coder, "debugging", t);
	if (signal)
		return (signal);
	usleep(coder->db_time);
	signal = print_action(coder, "refactoring", t);
	if (signal)
		return (signal);
	usleep(coder->refac_time);
	coder->comp_times += 1;
	return (signal);
}

static void	final_part(t_c_args *c_args, t_coder *coder)
{
	int	signal;

	if (*(coder->poison))
		return ;
	signal = safe_mutex_lock(c_args->begin_mtx);
	if (signal)
		return ;
	*c_args->coder_ready += 1;
	signal = safe_mutex_unlock(c_args->begin_mtx);
	return ;
}

void	*coder_rutine(void *args)
{
	t_coder			*coder;
	struct timeval	*t;
	int 			signal;

	signal = barrier_wait((t_c_args *)args);
	if (signal)
		return (NULL);
	coder = ((t_c_args *)args)->coder;
	t = ((t_c_args *)args)->t;
	while (coder->comp_times < coder->cycles && *(coder->poison) == 0)
	{
		signal = coder_loop_one(coder, t);
		if (signal)
			break ;
		signal = coder_loop_two(coder, t);
		if (signal)
			break ;
	}
	final_part((t_c_args *)args, coder);
	return (NULL);
}

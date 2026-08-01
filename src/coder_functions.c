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

static int	coder_loop_one(t_coder *coder, t_c_args *ar)
{
	int	drop_signal;

	if (take(coder, coder->dongles[0], ar))
		return (1);
	drop_signal = take(coder, coder->dongles[1], ar);
	if (drop_signal == 3)
	{
		if (drop(coder->dongles[0], coder))
			return (1);
		return (3);
	}
	else if (drop_signal)
		return (1);
	if (print_take(coder, ar))
		return (1);
	if (print_take(coder, ar))
		return (1);
	return (0);
}

static int	coder_loop_two(t_coder *coder, t_c_args *ar)
{
	if (act(coder, "compiling", ar))
		return (1);
	if (safe_mutex_lock(&coder->seal))
		return (1);
	if (safe_gettimeofday(&coder->last_compile_start))
		return (safe_mutex_unlock(&coder->seal, 1));
	if (safe_mutex_unlock(&coder->seal, 0))
		return (1);
	usleep(coder->compt_time);
	return (0);
	if (release(coder->dongles[0], ar, coder))
		return (1);
	if (release(coder->dongles[1], ar, coder))
		return (1);
	if (act(coder, "debugging", ar))
		return (1);
	usleep(coder->db_time);
	if (act(coder, "refactoring", ar))
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
	if (safe_mutex_unlock(c_args->begin_mtx, 0))
		return (1);
	return (0);
}

static int	special_coder_uwu(t_coder *coder, t_c_args *ar)
{
	int	c_num;

	c_num = ar->coder_num;
	return (coder->comp_times > 0 && coder->n_id == c_num && c_num > 1);
}

void	*coder_routine(void *args)
{
	t_coder			*coder;
	t_c_args		*ar;
	int				signal;

	ar = (t_c_args *)args;
	signal = wait(ar->begin_mtx, ar->begin_cnd, ar->coder_ready, ar->coder_num);
	if (signal)
		return ((void *)(intptr_t)signal);
	coder = ar->coder;
	while (coder->comp_times < coder->cycles && !(*(coder->poison)))
	{
		signal = coder_loop_one(coder, ar);
		if (signal == 3)
			continue ;
		else if (signal)
			return ((void *)(intptr_t)signal);
		signal = coder_loop_two(coder, ar);
		if (signal)
			return ((void *)(intptr_t)signal);
		if (special_coder_uwu(coder, ar))
			ft_pswap((void **)&coder->dongles[0], (void **)&coder->dongles[1]);
	}
	final_part((t_c_args *)args, coder);
	return (NULL);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 14:33:58 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/20 14:46:16 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "utils/utils.h"

int	coders_working(t_args *args)
{
	return (args->coder_ready >= 0 && args->coder_ready < args->data[0]);
}

int	burnout(t_args *args, t_coder *coders)
{
	int				i;
	suseconds_t		burnout;
	struct timeval	t;

	i = 0;
	t = args->ref_t[1];
	burnout = args->data[1];
	while (i < args->data[0])
	{
		if (t_diff(t, coders[i].last_compile_start) >= burnout)
		{
			args->burnt_coder = coders[i].n_id;
			args->poison = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

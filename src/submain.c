/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   submain.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 22:15:14 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/24 22:15:49 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int	adding_parsed_data(char **argv, t_args *args)
{
	int	*data;

	data = parser(argv);
	if (!data)
	{
		fprintf(stderr, "Memory allocation failed for parsed arguments.\n");
		return (1);
	}
	args->data = data;
	return (0);
}

int	set_the_table(char **argv, t_args *args)
{
	if (adding_parsed_data(argv, args))
		return (1);
	if (dongle_n_coder_creator(args))
		return (1);
	if (init_cond(args))
		return (1);
	if (init_mutex(args))
		return (1);
	return (0);
}

void	pass_the_ref(t_args *args)
{
	int				i;
	struct timeval	ref;
	t_coder			*coders;

	coders = args->coders;
	i = 0;
	ref = args->ref_t[0];
	while (i < args->data[0])
	{
		coders[i].last_compile_start.tv_sec = ref.tv_sec;
		coders[i].last_compile_start.tv_usec = ref.tv_usec;
		i++;
	}
}

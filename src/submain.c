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

int	adding_parsed_data(char **argv, t_args *args)
{
	int	*data;

	data = parser(argv);
	if (!data)
	{
		printf("Memory allocation failed for parsed arguments.\n");
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
	t_dongle		*dongles;

	coders = args->coders;
	dongles = args->dongles;
	i = 0;
	ref = args->ref_t[0];
	while (i < args->data[0])
	{
		coders[i].ref = ref;
		coders[i].own = ref;
		coders[i].last_compile_start = ref;
		dongles[i].last_used = ref;
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 17:42:55 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/06/27 17:42:56 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	int				i;
	int				*data;
	char			*sched;
	struct s_coder	*coders;
	struct s_dongle	*dongles;

	if (argc != 9)
	{
		fprintf(stderr, "Error: invalid number of arguments.\n");
		return (1);
	}
	i = parser(argv, &data, &sched);
	if (i)
		printf("Data parsed correctly...\n");
	i = init_wrapper(&coders, &dongles, data);
	if (i)
	{
		printf("Coders and dongles initialized correctly\n");
		printf("The right dongle of the last coder is %p\n", coders[data[0] - 1].dongle_right);
		printf("The left dongle of the first coder is %p\n", coders[0].dongle_left);
	}
	return (i);
}

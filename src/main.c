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
#include "utils/utils.h"

int	main(int argc, char **argv)
{
	int		i;
	int		*args;
	char	*sched;

	if (argc != 9)
	{
		fprintf(stderr, "Error: invalid number of arguments.\n");
		return (1);
	}
	i = parser(argv, &args, &sched);
	if (i)
		printf("Yay!");
	return (i);
}

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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

t_args	*argumenting(char **argv, int **data, char **sched)
{
	t_args	*args;

	if (!parser(argv, data, sched))
		return (NULL);
	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
	{
		fprintf(stderr, "Error while allocating memory to begin codexion.\n");
		return (NULL);
	}
	args->data = *data;
	args->sched = *sched;
	return (args);
}

int	main(int argc, char **argv)
{
	int			*data;
	char		*sched;
	t_args		*args;
	pthread_t	monitor;

	if (argc != 9)
	{
		fprintf(stderr, "Error: invalid number of arguments.\n");
		return (1);
	}
	args = argumenting(argv, &data, &sched);
	if (!args)
		return (1);
	pthread_create(&monitor, NULL, run_codexion, args);
	pthread_join(monitor, NULL);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 17:36:25 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/06/27 17:36:32 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

static int	sched_cmp(char *sched, int **arg_list)
{
	int	ok;

	ok = strcmp(sched, "fifo");
	if (ok == 0)
	{
		(*arg_list)[7] = 0;
		return (1);
	}
	ok = strcmp(sched, "edf");
	if (ok == 0)
	{
		(*arg_list)[7] = 1;
		return (1);
	}
	fprintf(stderr, "Error: Scheduler must be 'fifo' or 'edf'.\n");
	return (0);
}

static int	just_numbers(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	just_positives(int *args)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (args[i] < 0)
		{
			fprintf(stderr, "Argument %d is not a valid integer\n", args[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	from_argv_to_data(char **argv, int **data)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (just_numbers(argv[i]))
			(*data)[i - 1] = atoi(argv[i]);
		else
		{
			fprintf(stderr, "Argument %s invalid. Not an interger\n", argv[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	*parser(char **argv)
{
	int	*data;

	data = (int *)malloc(sizeof(int) * 8);
	if (data == NULL)
	{
		fprintf(stderr, "Error: memory allocation failed during parsing.\n");
		return (NULL);
	}
	if (!from_argv_to_data(argv, &data))
	{
		free(data);
		return (NULL);
	}
	if (!just_positives(data))
	{
		free(data);
		return (NULL);
	}
	if (!sched_cmp(argv[8], &data))
	{
		free(data);
		return (NULL);
	}
	return (data);
}

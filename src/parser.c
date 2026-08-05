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
#include <limits.h>
#include "utils/utils.h"

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

int	codexion_atoi(char *str)
{
	int	i;
	int	digit;
	int	result;
	int	cycles;

	i = 0;
	result = 0;
	if (!just_numbers(str))
		return (-1);
	cycles = strlen(str);
	while (i < cycles)
	{
		digit = str[i] - '0';
		if (result > (INT_MAX - digit) / 10)
			return (-1);
		result = (result * 10) + digit;
		i++;
	}
	return (result);
}

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

int	from_argv_to_data(char **argv, int **data)
{
	int	i;
	int	num;

	i = 1;
	while (i < 8)
	{
		num = codexion_atoi(argv[i]);
		if (num >= 0)
			(*data)[i - 1] = num;
		else
		{
			fprintf(stderr, "Argument %s is not a valid interger.\n", argv[i]);
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
	if (!sched_cmp(argv[8], &data))
	{
		free(data);
		return (NULL);
	}
	return (data);
}

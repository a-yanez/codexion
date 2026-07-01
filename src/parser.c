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
#include "utils/utils.h"

int	sched_cmp(char *sched)
{
	int	ok;

	ok = strcmp(sched, "fifo");
	if (ok == 0)
		return (1);
	ok = strcmp(sched, "edf");
	if (ok == 0)
		return (1);
	fprintf(stderr, "Error: Scheduler must be 'fifo' or 'edf'.\n");
	return (0);
}

int	just_numbers(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[0]) || str[0] != '+')
			return (0);
		i++;
	}
	return (1);
}

int	just_positives(int *args)
{
	int	i;

	i = 0;
	while (++i < 8)
	{
		if (args[i] < 0)
		{
			fprintf(stderr, "Argument %d is not a valid integer\n", args[i]);
			return (0);
		}
	}
	return (1);
}

int	parser(char **argv, int **arg_list, char **sched)
{
	int	i;

	i = 0;
	*arg_list = (int *)malloc(sizeof(int) * 7);
	if (*arg_list == NULL)
	{
		fprintf(stderr, "Error: memory allocation failed during parsing.\n");
		return (0);
	}
	fprintf(stdout, "Going through the array\n");
	while (++i < 8)
	{
		if (just_numbers(argv[i]))
			(*arg_list)[i - 1] = atoi(argv[i]);
		else
		{
			fprintf(stderr, "Argument %s invalid. Not an interger\n", argv[i]);
			return (0);
		}
	}
	sched = &argv[8];
	if (just_positives(*arg_list) != 1 || sched_cmp(*sched) != 1)
		return (0);
	return (1);
}

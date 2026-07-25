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
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

t_args	*argumenting(void)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
	{
		fprintf(stderr, "Error while allocating memory for arguments.\n");
		return (NULL);
	}
	args->coder_ready = 0;
	args->poison = 0;
	args->cnd_init = 0;
	args->mtx_init = 0;
	args->data = NULL;
	args->coders = NULL;
	args->dongles = NULL;
	args->c_args = NULL;
	return (args);
}

int	threading_coders(t_args *args)
{
	t_coder		*coders;
	t_c_args	*c_args;
	int			i;

	coders = args->coders;
	c_args = args->c_args;
	i = 0;
	while (i < args->data[0])
	{
		if (sf_thread_create(&coders[i].thread_id, coder_routine, &c_args[i]))
			return (1);
		i++;
	}
	return (0);
}

int	joining_workers(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->data[0])
	{
		if (safe_thread_join(args->coders[i].thread_id))
			return (1);
		i++;
	}
	return (0);
}

void	run_codexion(t_args *args)
{
	t_c_args	*c_args;
	pthread_t	monitor;

	c_args = create_c_args(args);
	if (!c_args)
		return ;
	args->c_args = c_args;
	if (safe_gettimeofday(&(args->ref_t[0])))
		return ;
	pass_the_ref(args);
	if (threading_coders(args))
		return ;
	if (sf_thread_create(&monitor, monitor_routine, args))
		return ;
	if (safe_thread_join(monitor))
		return ;
	if (joining_workers(args))
		return ;
	return ;
}

int	main(int argc, char **argv)
{
	t_args		*args;

	if (argc != 9)
	{
		fprintf(stderr, "Error: invalid number of arguments.\n");
		return (1);
	}
	args = argumenting();
	if (!args)
		return (1);
	if (set_the_table(argv, args))
		return (cleaning(args));
	run_codexion(args);
	cleaning(args);
	return (0);
}

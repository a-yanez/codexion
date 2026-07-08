/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 15:58:24 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/08 15:58:27 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "codexion.h"

void	*run_codexion(void *args)
{
	int				i;
	int				*data;
	struct s_coder	*coders;
	struct s_dongle	*dongles;

	data = ((t_args*)args)->data;
	i = init_wrapper(&coders, &dongles, data);
	if (i)
	{
		printf("Coders and dongles initialized correctly\n");
		printf("The right dongle of coder # %d is %p\n", coders[data[0] - 1].n_id, coders[data[0] - 1].dongle_right);
		printf("The left dongle of code # %d coder is %p\n", coders[0].n_id,  coders[0].dongle_left);
	}
	else
		printf("I died!\n");
	return (NULL);
}

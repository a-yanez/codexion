/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 17:36:01 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/06/27 17:36:12 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Coder structure
typedef struct s_dongle
{
	pthread_mutex_t	lock;
	int				cool_down;
	struct timeval	time;
}	t_dongle;

typedef struct s_coder
{
	pthread_t	id;
	int			*dongle_right;
	int			*dongle_left;
	int			cycles;
}	t_coder;

typedef struct s_monitor
{
	pthread_t	*id_mon;
}	t_monitor;

// parser file
int	parser(char **argv, int **arg_list, char **sched);

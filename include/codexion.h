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

#ifndef CODEXION_H
# define CODEXION_H

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

// Coder structure
typedef struct s_dongle
{
	pthread_mutex_t	lock;
	struct timeval	time;
	int				cool_down;
}	t_dongle;

typedef struct s_coder
{
	int				n_id;
	int				cycles;
	pthread_t		thread_id;
	struct s_dongle	*dongle_right;
	struct s_dongle	*dongle_left;
}	t_coder;

typedef struct s_monitor
{
	pthread_t	*id_mon;
}	t_monitor;

//Args structure
typedef struct s_args
{
	int		*data;
	char	*sched;
}	t_args;

// parser functions
int	parser(char **argv, int **arg_list, char **sched);

// initializer functions
int	init_wrapper(t_coder **coders, t_dongle **dongles, int *data);

//monitor functions
void	*run_codexion(void *args);

#endif
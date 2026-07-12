/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 17:36:01 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/12 00:13:51 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

// Coders and dongles structure
typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	struct timeval	last_used;
	int				cool_down;
	int				avail;
	int				edf;
	t_coder			*queue[2];
}	t_dongle;

typedef struct s_coder
{
	int				n_id;
	int				cycles;
	pthread_t		thread_id;
	int				compt_time;
	int				db_time;
	int				refac_time;
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
int		parser(char **argv, int **arg_list, char **sched);

// initializer functions
void	free_dongles(t_dongle **dongles, int current);
void	free_coders(t_coder **coders, int current);
int		init_wrapper(t_coder **coders, t_dongle **dongles, int *data);

//monitor functions
void	*run_codexion(void *args);

#endif

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
	suseconds_t 	last_used;
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
	suseconds_t 	last_compile_start;
}	t_coder;

typedef struct s_monitor
{
	pthread_t	*id_mon;
}	t_monitor;

//Args structure
typedef struct s_args
{
	int				*data;
	char			*sched;
	pthread_mutex_t	*printer;
}	t_args;

// parser functions
int		parser(char **argv, int **arg_list, char **sched);

// initializer functions
int		init_wrapper(t_coder **coders, t_dongle **dongles, int *data, char *sched);

//monitor functions
void	*run_codexion(void *args);

//dongle functions
void	edf(t_dongle *dongle);
void	queue(t_dongle *dongle, t_coder *coder);
void	pop(t_dongle *dongle);

#endif

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

# include <bits/types/struct_timeval.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <time.h>

// Coders and dongles structure
typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	lock;
	pthread_cond_t	cond;
	struct timeval	last_used;
	suseconds_t		cool_down;
	int				on_use;
	int				edf;
	struct timespec	ts;
	t_coder			*queue[2];

}	t_dongle;

typedef struct s_coder
{
	int				n_id;
	int				cycles;
	int				comp_times;
	pthread_t		thread_id;
	suseconds_t		compt_time;
	suseconds_t		db_time;
	suseconds_t		refac_time;
	struct s_dongle	*dongles[2];
	suseconds_t		burnout;
	struct timeval	last_compile_start;
	struct timeval	*ref;
	pthread_mutex_t	*printer;
}	t_coder;

typedef struct s_monitor
{
	pthread_t	*id_mon;
}	t_monitor;

//Args structure
typedef struct s_monitor_args
{
	int				*data;
	char			*sched;
	int				coder_ready;
	pthread_mutex_t	begin_mtx;
	pthread_cond_t	begin_cnd;
	pthread_mutex_t	printer;
}	t_args;

typedef struct s_coder_arguments
{
	t_coder			*coder;
	struct timeval	*t;
	int				*coder_ready;
	int				*coder_num;
	pthread_mutex_t	*begin_mtx;
	pthread_cond_t	*begin_cnd;
}	t_c_args;

// parser functions
int		parser(char **argv, int **arg_list, char **sched);

// initializer functions
int		init_wrapper(t_coder **coders, t_dongle **dongles, t_args *args);

//monitor functions
void	*run_codexion(void *args);

//coder functions
void	barrier_wait(t_c_args *c_args);
void	*coder_rutine(void *args);

//dongle functions
void	edf(t_dongle *dongle);
void	queue(t_dongle *dongle, t_coder *coder);
void	pop(t_dongle *dongle);

#endif

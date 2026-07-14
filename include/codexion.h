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
	int				on_use;
	int				edf;
	t_coder			*queue[2];
}	t_dongle;

typedef struct s_coder
{
	int				n_id;
	int				cycles;
	pthread_t		thread_id;
	suseconds_t		compt_time;
	suseconds_t		db_time;
	suseconds_t		refac_time;
	struct s_dongle	*dongles[2];
	suseconds_t		burnout;
	suseconds_t 	last_compile_start;
	pthread_mutex_t	*printer;
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
int	init_wrapper(t_coder **coders, t_dongle **dongles, t_args *args);

//monitor functions
void	*run_codexion(void *args);

//dongle functions
void	edf(t_dongle *dongle);
void	queue(t_dongle *dongle, t_coder *coder);
void	pop(t_dongle *dongle);

#endif

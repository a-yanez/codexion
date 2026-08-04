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
# include <inttypes.h>
# include <pthread.h>
# include <sys/time.h>
# include <time.h>

// Coders and dongles structure
typedef struct s_dongle				t_dongle;
typedef struct s_coder				t_coder;
typedef struct s_coder_arguments	t_c_args;
typedef struct timeval				t_tmval;

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
	int				holding;
	int				finished;
	int				*poison;
	pthread_t		thread_id;
	suseconds_t		compt_time;
	suseconds_t		db_time;
	suseconds_t		refac_time;
	struct s_dongle	*dongles[2];
	struct timeval	last_compile_start;
	struct timeval	ref;
	struct timeval	own;
	pthread_mutex_t	*printer;
	pthread_mutex_t	seal;
}	t_coder;

//Args structure
typedef struct s_general_args
{
	int				*data;
	int				poison;
	int				coder_ready;
	int				coder_done;
	int				burnt_coder;
	int				mtx_init;
	int				cnd_init;
	int				cdr_mtx;
	int				dng_mtx;
	struct timeval	ref_t[2];
	t_dongle		*dongles;
	t_coder			*coders;
	t_c_args		*c_args;
	pthread_mutex_t	begin_mtx;
	pthread_cond_t	begin_cnd;
	pthread_mutex_t	printer;
}	t_args;

typedef struct s_coder_arguments
{
	t_coder			*coder;
	int				*coder_ready;
	int				*coder_done;
	int				coder_num;
	int				burnout;
	pthread_mutex_t	*begin_mtx;
	pthread_cond_t	*begin_cnd;
}	t_c_args;

// main functions
t_args		*argumenting(void);

// parser functions
int			*parser(char **argv);

//submain functions
int			adding_parsed_data(char **argv, t_args *args);
int			set_the_table(char **argv, t_args *args);
void		pass_the_ref(t_args *args);

// dongle and coder creation functions
int			dongle_n_coder_creator(t_args *args);
t_c_args	*create_c_args(t_args *args);

//conds and mutex
int			destroy_conds(t_args *args);
int			init_cond(t_args *args);
int			destroy_mutex(t_args *args);
int			init_mutex(t_args *args);

//monitor functions
void		*monitor_routine(void *args);

//monitor utils
int			wait(pthread_mutex_t *m, pthread_cond_t *c, int *ready, int num);
int			coders_working(t_args *args);
int64_t		calculate_delta(t_coder *coder, t_tmval t);
int			is_coder_done(t_coder *coder);

//coder functions
void		*coder_routine(void *args);

//coder utils
int			check_poison(pthread_mutex_t *b_mtx, int *poison);
int			print_take(t_coder *c, t_c_args *ar);
int			act(t_coder *c, char *ac, t_c_args *ar);
int			take(t_coder *c, t_dongle *d, t_c_args *a);
int			release(t_dongle *dongle, t_c_args *ar, t_coder *coder);
int			drop(t_dongle *dongle, t_coder *coder);

//dongle functions
int			edf(t_dongle *dongle);
int			queue(t_dongle *dongle, t_coder *coder);
void		pop(t_dongle *dongle);

// cleaning
int			cleaning(t_args *args);

#endif

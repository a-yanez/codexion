/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 18:25:26 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/01 18:25:29 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "codexion.h"
# include <time.h>

// char analysis functions
int		ft_isdigit(int c);

// pointer swapper
void	ft_pswap(void **a, void **b);

//Function to free memory
void	free_dongles(t_dongle **dongles, int current);
void	free_coders(t_coder **coders, int current);
void	free_both(t_coder **coders, t_dongle **dongles, int current);

// time functions
int		safe_gettimeofday(struct timeval *t);
int		set_timeout(struct timespec *ts, int timeout_ms);
long	t_diff(struct timeval tv1, struct timeval tv2);

//safe thread functions
int		safe_thread_create(pthread_t t_id, void *(*routine)(void *), void *arg);
int		safe_thread_join(pthread_t thread_id);
int		safe_mutex_init(pthread_mutex_t mutex);
int		safe_cond_init(pthread_cond_t cond);
int		safe_mutex_destroy(pthread_mutex_t *mutex);
int		safe_cond_destroy(pthread_cond_t *cond);
int		safe_mutex_lock(pthread_mutex_t *mutex);
int		safe_mutex_unlock(pthread_mutex_t *mutex);
int		safe_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int		s_tmwt(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *t);
int		safe_cond_signal(pthread_cond_t *cond);
int		safe_cond_broadcast(pthread_cond_t *cond);

#endif

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

// time functions
void	set_timeout(struct timespec *ts, int timeout_ms);
long	t_diff(struct timeval tv1, struct timeval tv2);

#endif

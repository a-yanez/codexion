/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o <ayanez-o@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 17:21:23 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/09 17:21:26 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/utils.h"
#include "codexion.h"
#include <stdlib.h>

void	free_dongles(t_dongle **dongles, int current)
{
	while (current >= 0)
	{
		free(&((*dongles)[current]));
		current--;
	}
}

void	free_coders(t_coder **coders, int current)
{
	while (current >= 0)
	{
		free(&((*coders)[current]));
		current--;
	}
}

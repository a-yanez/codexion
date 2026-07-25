/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayanez-o@student.42barcelona.com           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 23:28:05 by ayanez-o          #+#    #+#             */
/*   Updated: 2026/07/24 23:28:12 by ayanez-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

int	cleaning(t_args *args)
{
	if (args->c_args != NULL)
		free(args->c_args);
	if (args->cnd_init > 0)
		destroy_conds(args);
	if (args->mtx_init > 0)
		destroy_mutex(args);
	if (args->coders != NULL)
		free(args->coders);
	if (args->dongles != NULL)
		free(args->dongles);
	if (args->data != NULL)
		free(args->data);
	free(args);
	return (0);
}

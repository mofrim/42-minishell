/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_redirlst_print.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 09:39:02 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 09:39:30 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Print the redirlst for debugging. */
void	redirlst_print(t_redirlst *lst)
{
	while (lst)
	{
		ft_printf("{type: %d, ", lst->redtype);
		ft_printf("fd_in: %d, ", lst->fd_from);
		ft_printf("fd_out: %d, ", lst->fd_to);
		ft_printf("infile: %s, ", lst->infile);
		ft_printf("outfile: %s}\n", lst->outfile);
		lst = lst->next;
	}
}

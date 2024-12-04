/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_exit_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:35:30 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/14 23:37:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

void	error_exit(char *msg)
{
	if (msg)
		perror(msg);
	else
		perror("Error");
	exit(EXIT_FAILURE);
}

/* Perform NULL check on pointer. error_exit with msg if NULL. */
void	nullcheck(void *p, char *msg)
{
	if (!p)
		error_exit(msg);
}

/* Exit with error custom error msg, or not. */
// void	error_exit_free(char *msg, t_ppx **ppx)
// {
// 	if (msg)
// 		perror(msg);
// 	else
// 		perror("Error");
// 	free_ppx(ppx);
// 	exit(EXIT_FAILURE);
// }



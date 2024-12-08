/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:37 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/08 01:04:39 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bltin_echo(char **arg, t_envlst **el)
{
	(void)arg;
	(void)el;
	ft_printf(RED "<< DEBUG >> bltin_echo not yet implemented!\n" RST);
	return (0);
}

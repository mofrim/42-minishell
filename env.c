/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:27:28 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/02 19:12:18 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	print_env(char **env)
{
	while(*env)
	{
		ft_printf("env: %s\n", *env);
		env++;
	}
}

void	print_envlst(t_envlst *el)
{
	while (el)
	{
		ft_printf("env name: %s, value: %s\n", el->name, el->value);
		el = el->next;
	}
}

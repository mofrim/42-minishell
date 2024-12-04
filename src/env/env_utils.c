/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:27:28 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 15:47:29 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(char **env)
{
	while (*env)
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

/* Get the value-string for a env-var name from envlst. */
char	*get_env_value(char *name, t_envlst *el)
{
	while (el)
	{
		if (!strcmp(name, el->name))
			return (el->value);
		el = el->next;
	}
	return (NULL);
}

/* Get ptr to envlst-entry by name. */
t_envlst	*get_env_entry_by_name(char *name, t_envlst *el)
{
	while (el)
	{
		if (!strcmp(name, el->name))
			return (el);
		el = el->next;
	}
	return (NULL);
}

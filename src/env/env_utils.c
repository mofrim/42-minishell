/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:27:28 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 17:53:40 by fmaurer          ###   ########.fr       */
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
		if (ft_strcmp(el->name, "?") && \
	!(!ft_strcmp(el->name, "PATH") && !ft_strcmp(el->value, "/no-such-path")))
		{
			if (el->value)
				ft_printf("declare -x %s=\"%s\"\n", el->name, el->value);
			else
				ft_printf("declare -x %s\n", el->name);
		}
		el = el->next;
	}
}

/* Get the value-string for a env-var name from envlst. */
char	*get_env_value(char *name, t_envlst *el)
{
	while (el)
	{
		if (!ft_strcmp(name, el->name))
			return (el->value);
		el = el->next;
	}
	return (NULL);
}

/* Get ptr to envlst-entry by name. */
t_envlst	*get_env_entry_by_name(char *name, t_envlst *el)
{
	if (name == NULL)
		return (NULL);
	while (el)
	{
		if (!ft_strcmp(name, el->name))
			return (el);
		el = el->next;
	}
	return (NULL);
}

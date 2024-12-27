/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get_env_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 14:43:30 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 23:44:53 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*join_env_entry(t_envlst *el);

char	**get_env_array(t_envlst *el)
{
	int		lstsize;
	int		i;
	char	**env;

	lstsize = envlst_size(el);
	env = malloc(sizeof(char *) * (lstsize + 1));
	if (!env)
		return (NULL);
	i = -1;
	while (++i < lstsize)
	{
		env[i] = join_env_entry(el);
		el = el->next;
	}
	env[lstsize] = NULL;
	return (env);
}

static char	*join_env_entry(t_envlst *el)
{
	char	*tmp;
	char	*joi;

	tmp = ft_strjoin(el->name, "=");
	joi = ft_strjoin(tmp, el->value);
	free(tmp);
	return (joi);
}

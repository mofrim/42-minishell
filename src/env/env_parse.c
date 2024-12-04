/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:11:04 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 14:40:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	splitsize(char **s)
{
	int	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

/* 
 * FIXME: dirty hack for re-joining splits of env-var values that contained '='
 * characters. definitely would be better to write a specified ft_split version
 * that only splits at the first occurrence of '='.
 */
static char	*splitjoin(char **s)
{
	char	*joi;
	char	*tmp;
	char	*tmp1;

	joi = NULL;
	while (*s)
	{
		tmp = joi;
		joi = ft_strjoin(joi, *s);
		free(tmp);
		s++;
		if (*s)
		{
			tmp = joi;
			joi = ft_strjoin(joi, "=");
			free(tmp);
		}
	}
	return (joi);
}

static void	freesplit(char **s)
{
	while (*s)
	{
		free(*s);
		s++;
	}
}

/* Parse the ENV into a linked list for future extension. */
t_envlst	*parse_env(char **env)
{
	char		**var;
	t_envlst	*elst;
	t_envlst	*enew;

	elst = NULL;
	while (*env)
	{
		var = ft_split(*env, '=');
		if (splitsize(var) <= 2)
			enew = envlst_new(ft_strdup(var[0]), ft_strdup(var[1]));
		else
			enew = envlst_new(ft_strdup(var[0]), splitjoin(&var[1]));
		envlst_add_back(&elst, enew);
		freesplit(var);
		free(var);
		env++;
	}
	return (elst);
}

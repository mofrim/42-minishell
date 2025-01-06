/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:11:04 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 13:14:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*splitjoin(char **s)
{
	char	*joi;
	char	*tmp;

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

/* 
 * FIXME: dirty hack for re-joining splits of env-var values that contained '='
 * characters. definitely would be better to write a specified ft_split version
 * that only splits at the first occurrence of '='.
 */

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
	char		*sjoin;

	elst = NULL;
	while (*env)
	{
		var = ft_split(*env, '=');
		if (splitsize(var) <= 2)
			enew = envlst_new(var[0], var[1]);
		else
		{
			sjoin = splitjoin(&var[1]);
			enew = envlst_new(var[0], sjoin);
			free(sjoin);
		}
		envlst_add_back(&elst, enew);
		freesplit(var);
		free(var);
		env++;
	}
	return (elst);
}

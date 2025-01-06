/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 04:34:46 by elpah             #+#    #+#             */
/*   Updated: 2025/01/06 10:16:39 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Find the name in a export arg string like `varname=value`. */
char	*find_name(char *str, char *equal_pos)
{
	char	*result;
	int		copy_len;

	copy_len = equal_pos - str + 1;
	result = malloc(copy_len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, copy_len);
	return (result);
}

/* Check envvar name for invalid chars. */
int	is_valid_identifier(const char *str)
{
	int	i;

	if ((!ft_isalpha(str[0]) && str[0] != '_') || *str == 0)
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	**ft_split_input(char *str)
{
	char	**result;
	char	*equal_pos;

	result = malloc(2 * sizeof(char *));
	if (!result)
		return (NULL);
	equal_pos = ft_strchr(str, '=');
	if (!equal_pos)
	{
		result[0] = ft_strdup(str);
		result[1] = NULL;
	}
	else
	{
		result[0] = find_name(str, equal_pos);
		if (!result[0])
			return (free(result), NULL);
		result[1] = ft_strdup(equal_pos + 1);
		if (!result[1])
			return (free(result[0]), free(result), NULL);
	}
	return (result);
}

// Swap Env
void	swap_env_vars(t_envlst *a, t_envlst *b)
{
	char	*temp_name;
	char	*temp_value;

	temp_name = a->name;
	temp_value = a->value;
	a->name = b->name;
	a->value = b->value;
	b->name = temp_name;
	b->value = temp_value;
}

// Sort env
void	sort_env_list(t_envlst *env)
{
	t_envlst	*i;
	t_envlst	*j;

	i = env;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (ft_strcmp(i->name, j->name) > 0)
				swap_env_vars(i, j);
			j = j->next;
		}
		i = i->next;
	}
}


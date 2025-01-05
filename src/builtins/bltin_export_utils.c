/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 04:34:46 by elpah             #+#    #+#             */
/*   Updated: 2025/01/05 21:25:22 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	is_valid_identifier(const char *str)
{
	int	i;

	if (!ft_isalpha(str[0]) && str[0] != '_')
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

int	check_valid_vars(char **args)
{
	int	i;
	int	j;
	int	error;

	i = 1;
	j = 1;
	error = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_printf("export: `%s': not a valid identifier\n", args[i]);
			free(args[i]);
			error = 1;
		}
		else
		{
			if (i != j)
				args[j] = args[i];
			j++;
		}
		i++;
	}
	args[j] = NULL;
	return (error);
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

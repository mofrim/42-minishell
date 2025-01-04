/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 04:34:46 by elpah             #+#    #+#             */
/*   Updated: 2025/01/04 05:00:51 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	counter;

	counter = 0;
	while (src[counter] != '\0' && counter < n)
	{
		dest[counter] = src[counter];
		counter++;
	}
	while (counter < n)
	{
		dest[counter] = '\0';
		counter++;
	}
	return (dest);
}

char	*find_name(char *str, char *equal_pos)
{
	char	*result;

	result = malloc(equal_pos - str + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str, equal_pos - str);
	result[equal_pos - str] = '\0';
	return (result);
}

void	handle_invalid(char **args, int i)
{
	int	j;

	j = i;
	ft_printf("export: `%s': not a valid identifier\n", args[i]);
	while (args[j] != NULL)
	{
		args[j] = args[j + 1];
		j++;
	}
}

void	handle_backslash(char **args, int i)
{
	int	k;

	k = 0;
	if (args[i][1] == '\\')
	{
		handle_invalid(args, i);
		return ;
	}
	else if (ft_isalpha(args[i][1]) || args[i][1] == '_'
			|| args[i][1] == '-' || args[i][1] == '\0')
	{
		while (args[i][k] != '\0')
		{
			args[i][k] = args[i][k + 1];
			k++;
		}
	}
	else
		handle_invalid(args, i);
}

void	handle_dash(char **args, int i)
{
	int	j;

	j = i;
	if (args[i][0] == '-' && args[i][1] != '\0'
		&& !ft_strchr("nfp", args[i][1]))
	{
		ft_printf("export: -%c: invalid option\n", args[i][1]);
		ft_printf("export: usage: export ");
		ft_printf("[-nf][name[= value]...] or export -p\n");
		while (args[j] != NULL)
		{
			args[j] = args[j + 1];
			j++;
		}
	}
}

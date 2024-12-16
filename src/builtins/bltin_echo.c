/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:37 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 10:28:32 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	array_length(char **str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i] != NULL)
		i++;
	return (i);
}

int	bltin_echo(char **s)
{
	int		i;
	char	**str;

	i = 0;
	if (array_length(s) == 1)
	{
		ft_printf("\n");
		return (0);
	}
	str = &s[1];
	if (str)
	{
		if (!ft_strncmp(str[i], "-n", 2) && ft_strlen(str[i]) == 2)
			str = &str[1];
		while (str[i])
		{
			ft_printf("%s", str[i++]);
			if (str[i])
				ft_printf(" ");
		}
	}
	if (!(!ft_strncmp(s[1], "-n", 2) && ft_strlen(s[1]) == 2))
		ft_printf("\n");
	return (0);
}

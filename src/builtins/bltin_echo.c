/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:37 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/22 22:54:01 by fmaurer          ###   ########.fr       */
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

int	bltin_echo(t_cmdlst *cl, t_envlst **el)
{
	int		i;
	char	**str;
	char	**args;

	(void)el;
	args = cl->args;
	i = 0;
	if (array_length(args) == 1)
	{
		ft_printf("\n");
		return (0);
	}
	str = &args[1];
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
	if (!(!ft_strncmp(args[1], "-n", 2) && ft_strlen(args[1]) == 2))
		ft_printf("\n");
	return (0);
}

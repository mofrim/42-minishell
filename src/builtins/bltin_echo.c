/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:37 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 09:58:03 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(char **args, int i)
{
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
}

int	check_n_print(char **args)
{
	int	i;
	int	j;
	int	n_flag;

	n_flag = 0;
	i = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			n_flag = 1;
			i++;
		}
		else
			break ;
	}
	print_args(args, i);
	return (n_flag);
}

int	bltin_echo(t_cmdlst *cl, t_envlst **el)
{
	int	n_flag;

	n_flag = 0;
	(void)el;
	if (cl->arg_count == 1)
		return (ft_printf("\n"), 0);
	n_flag = check_n_print(cl->args);
	if (!n_flag)
		ft_printf("\n");
	return (0);
}

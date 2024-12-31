/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:58 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/01 00:45:33 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_number(char *str)
{
	int i;

	i = 0;
	if (!str || *str == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

int bltin_exit(t_envlst **env, char **str)
{
	int exit_status;

	if (array_length(str) > 2)
	{
		ft_printf("minishell: exit: too many arguments\n");
		return (1);
	}
	if (array_length(str) == 2)
	{
		if (!is_number(str[1]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", str[1]);
			return (1);
		}
		exit_status = ft_atoi(str[1]);
		if (exit_status < 0 || exit_status > 255)
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", str[1]);
			return (1);
		}
	}
	else
		exit_status = 0;
	// free_environment(*env);
	ft_printf("exit\n");
	// return (exit_status);
	exit(exit_status);
	
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:46 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 10:51:47 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_envlst *el)
{
	while (el)
	{
		if (el->value)
			ft_printf("%s=\"%s\"\n", el->name, el->value);
		el = el->next;
	}
	return ;
}

int	bltin_env(t_envlst *env, char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i > 1)
	{
		ft_printf("minishell: env: %s: No such file or directory\n", str[1]);
		return (1);
	}
	print_env_vars(env);
	return (0);
}

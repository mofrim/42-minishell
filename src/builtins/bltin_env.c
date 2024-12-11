/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:46 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/11 12:58:40 by elpah            ###   ########.fr       */
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

int	env(t_envlst *env)
{
    print_env_vars(env);
    return (0);
}

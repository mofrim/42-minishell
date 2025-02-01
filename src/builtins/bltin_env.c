/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:46 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/01 14:38:26 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env_vars(t_envlst *el)
{
	while (el)
	{
		if (el->value)
			ft_printf("%s=%s\n", el->name, el->value);
		el = el->next;
	}
	return ;
}

int	bltin_env(t_cmdlst *cl, t_envlst **el)
{
	if (cl->arg_count >= 2)
	{
		if (cl->args[1][0] == '-')
			return (minish_errormsg2("env", cl->args[1],
					"invalid option", 127));
		else
			return (minish_errormsg2("env", cl->args[1],
					"No such file or dir", 127));
	}
	print_env_vars(*el);
	return (0);
}

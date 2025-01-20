/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:46 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 17:54:37 by fmaurer          ###   ########.fr       */
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
	int	i;

	i = 0;
	while (cl->args[i])
		i++;
	if (i > 1)
	{
		ft_dprintf(2, "env: '%s': No such file or directory\n", cl->args[1]);
		return (127);
	}
	print_env_vars(*el);
	return (0);
}

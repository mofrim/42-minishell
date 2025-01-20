/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:14:32 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 18:29:20 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_free(t_envlst *ptr)
{
	if (!ptr)
		return ;
	free(ptr->name);
	free(ptr->value);
	free(ptr);
}

/* Unset... seems it doesn't know any errors. */
int	bltin_unset(t_cmdlst *cl, t_envlst **el)
{
	if (!el || !*el || cl->arg_count == 1)
		return (0);
	envlst_delone_by_name(cl->args[1], el);
	return (0);
}

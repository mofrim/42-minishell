/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 21:09:58 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/28 21:11:41 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Set cmdlst->args[0] to the full execution path of a cmd to be called. Return
 * 0 if nothing goes wrong, value != 0 otherwise.*/
int	set_exec_path(t_cmdlst *cl, char **env)
{
	char	*exec_path;

	exec_path = get_exec_path(cl, env);
	if (exec_path == NULL)
		return (minish_errormsg(cl->cmd, "command not found!", ENOENT));
	free(cl->args[0]);
	cl->args[0] = ft_strdup(exec_path);
	free(exec_path);
	return (0);
}


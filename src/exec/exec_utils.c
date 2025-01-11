/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 21:09:58 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/11 22:01:42 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Set cmdlst->args[0] to the full execution path of a cmd to be called. Return
 * 0 if nothing goes wrong, value != 0 otherwise.*/
int	set_exec_path(t_cmdlst *cl, char **env)
{
	char		*exec_path;
	struct stat	sb;

	if (cl->cmd == NULL && cl->redirs)
		return (0);
	exec_path = get_exec_path(cl, env);
	if (exec_path == NULL)
	{
		if (!stat(cl->cmd, &sb) && (sb.st_mode & S_IFDIR) == S_IFDIR)
			return (minish_errormsg(cl->cmd, "is a directory", 126));
		else
			return (minish_errormsg(cl->cmd, "command not found!", 127));
	}
	if (!ft_strcmp(exec_path, ""))
		return (free(exec_path), \
				minish_errormsg("permission denied", cl->cmd, 126));
	free(cl->args[0]);
	cl->args[0] = ft_strdup(exec_path);
	free(exec_path);
	return (0);
}

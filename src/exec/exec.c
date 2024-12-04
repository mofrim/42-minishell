/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 10:40:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME: first dirty sketch of a execution function. Will this work with Pipes
// and redirects?
int	exec_cmd(t_cmdlst *cmdl, char **env)
{
	char	*exec_path;
	int		cpid;

	if (cmdl->cmd == NULL)
		return (ENOENT);
	exec_path = get_exec_path(cmdl, env);
	if (exec_path == NULL)
	{
		ft_printf("%s: command not found!\n", cmdl->cmd);
		return (ENOENT);
	}
	free(cmdl->args[0]);
	cmdl->args[0] = exec_path;
	cpid = fork();
	if (cpid < 0)
		error_exit("fork failed");
	if (cpid == 0)
		execve(exec_path, cmdl->args, env);
	else
		waitpid(cpid, NULL, 0);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/03 21:24:37 by fmaurer          ###   ########.fr       */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:39:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 23:33:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_single_cmd(t_cmdlst *cmdl, char **env);

int	exec_single(t_cmdlst *cmdl, char **env, t_envlst **el)
{
	int	exit_status;

	exit_status = 0;
	if (cmdl->is_builtin)
		exit_status = exec_single_builtin_cmd(cmdl, el);
	else
		exit_status = exec_single_cmd(cmdl, env);
	return (exit_status);
}

/* Exec a single not pipelined cmd with redirs. */
int	exec_single_cmd(t_cmdlst *cmdl, char **env)
{
	char	*exec_path;
	int		cpid;
	int		status;

	exec_path = get_exec_path(cmdl, env);
	if (exec_path == NULL)
		return (minish_errormsg(cmdl->cmd, \
				"command not found!", ENOENT));
	free(cmdl->args[0]);
	cmdl->args[0] = ft_strdup(exec_path);
	free(exec_path);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_single_redir_cmd", \
					"fork failed!", errno));
	if (cpid == 0)
	{
		if (open_redir_files(cmdl->redirs))
			exit(errno);
		execve(cmdl->args[0], cmdl->args, env);
	}
	waitpid(cpid, &status, 0);
	return (status >> 8);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:39:14 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/22 11:31:08 by fmaurer          ###   ########.fr       */
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
int	exec_single_cmd(t_cmdlst *cl, char **env)
{
	int		cpid;
	int		status;
	int		err_exec_path;

	signal(SIGINT, SIG_IGN);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_single_redir_cmd", \
					"fork failed!", errno));
	if (cpid == 0)
	{
		if (open_redir_files(cl->redirs))
			exit(1);
		err_exec_path = set_exec_path(cl, env);
		if (err_exec_path)
			exit(err_exec_path);
		signal(SIGINT, SIG_DFL);
		if (cl->cmd)
			execve(cl->args[0], cl->args, env);
		exit(0);
	}
	waitpid(cpid, &status, 0);
	signal(SIGINT, sigint_handler);
	return (status);
}

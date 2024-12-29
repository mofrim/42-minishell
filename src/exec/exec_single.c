/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:39:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/29 10:54:05 by fmaurer          ###   ########.fr       */
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
	char	*exec_path;
	int		cpid;
	int		status;

	exec_path = get_exec_path(cl, env);
	if (set_exec_path(cl, env) != 0)
		return (ENOENT);
	signal(SIGINT, SIG_IGN);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_single_redir_cmd", \
					"fork failed!", errno));
	if (cpid == 0)
	{
		if (open_redir_files(cl->redirs))
			exit(errno);
		signal(SIGINT, sigint_handler);
		execve(cl->args[0], cl->args, env);
	}
	waitpid(cpid, &status, 0);
	signal_setup(sigint_handler);
	return (status >> 8);
}

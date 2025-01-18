/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 22:55:40 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 09:49:27 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The pipe redirects have to be done *before* all other redirs. */

static void	run_child(t_cmdlst *cl, int pipefd[2], int prev_read, char **env)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(prev_read, STDIN_FILENO);
	close(prev_read);
	if (open_redir_files(cl->redirs) != 0)
		exit(1);
	signal(SIGINT, SIG_DFL);
	if (cl->cmd)
		execve(cl->args[0], cl->args, env);
	exit(0);
}

int	exec_pipe_cmd(t_cmdlst *cl, char **env, int *prev_read)
{
	int		pipefd[2];
	pid_t	cpid;
	int		err_exec_path;

	if (pipe(pipefd) == -1)
		return (minish_errormsg("exec_pipe_cmd", "create pipe failed", errno));
	err_exec_path = set_exec_path(cl, env);
	if (err_exec_path)
		return (err_exec_path);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_pipe_cmd", "fork failed", errno));
	if (cpid == 0)
		run_child(cl, pipefd, *prev_read, env);
	else
	{
		close (pipefd[1]);
		close (*prev_read);
		*prev_read = pipefd[0];
	}
	return (cpid);
}

int	exec_pipe_cmd_last(t_cmdlst *cl, char **env, int prev_read)
{
	pid_t	cpid;
	int		err_exec_path;

	err_exec_path = set_exec_path(cl, env);
	if (err_exec_path)
		return (err_exec_path);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_pipe_cmd_last", "fork failed!", errno));
	if (cpid == 0)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
		if (open_redir_files(cl->redirs))
			exit(1);
		signal(SIGINT, sigint_handler);
		if (cl->cmd)
			execve(cl->args[0], cl->args, env);
		exit(0);
	}
	else
		close (prev_read);
	return (cpid);
}

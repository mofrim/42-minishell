/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 22:55:40 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/21 20:33:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * The child process routine for a normal command.
 *
 * As we have opened a new pipe connecting this and the next pipe process, we
 * close the read end `pipefd[0]` here and duplicate stdout to the write end
 * `pipefd[1]`. Stdin comes from the previous pipe cmd and is passed to the
 * current process via the fd vairable `prev_read.` So we need to duplicate
 * stdin to this variable. 
 * We are pausing singal handling in the main program by setting `signal(SIGINT,
 * SIG_IGN)` so in order to allow signal handling by the child process's own way
 * we need to reset `signal(SIGINT, SIG_DFL)`. This is necessary because ignored
 * signals are passed on as ignored signals to the new process. Signals that are
 * not ignored get to be handled.
 *
 * The pipe redirects have to be done *before* all other redirs! Otherwise pipes
 * would write to possible output redirections.
 */
static void	run_child(t_cmdlst *cl, int pipefd[2], int prev_read, char **env)
{
	int		err_exec_path;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(prev_read, STDIN_FILENO);
	close(prev_read);
	if (open_redir_files(cl->redirs) != 0)
		exit(1);
	err_exec_path = set_exec_path(cl, env);
	if (err_exec_path)
		exit(err_exec_path);
	signal(SIGINT, SIG_DFL);
	if (cl->cmd)
		execve(cl->args[0], cl->args, env);
	exit(0);
}

/**
 * Main routine for executing a cmd in the middle or start of a pipe.
 *
 * A new pipe is created connecting, this and the next pipe cmd. The pipe-global
 * variable `prev_read` is set to the read-end of the new pipe so it can be
 * passed on to the next process for being connected to the stdin in there.
 */
int	exec_pipe_cmd(t_cmdlst *cl, char **env, int *prev_read)
{
	int		pipefd[2];
	pid_t	cpid;

	if (pipe(pipefd) == -1)
		return (minish_errormsg("exec_pipe_cmd", "create pipe failed", 0));
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_pipe_cmd", "fork failed", 0));
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

/**
 * The last cmd in a pipeline.
 *
 * In the last cmd of a pipeline we don't need to create a new pipe bc there is
 * no next process to pipe anything to. Stdin comes via the `prev_read` var.
 */
int	exec_pipe_cmd_last(t_cmdlst *cl, char **env, int prev_read)
{
	pid_t	cpid;
	int		err_exec_path;

	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_pipe_cmd_last", "fork failed!", 0));
	if (cpid == 0)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
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
	else
		close (prev_read);
	return (cpid);
}

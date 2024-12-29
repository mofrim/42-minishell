/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 22:55:40 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/29 12:41:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The pipe redirects have to be done *before* all other redirs.
 * QUESTION: Why?*/
static void	run_child(t_cmdlst *cl, int pipefd[2], int prev_read, char **env)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(prev_read, STDIN_FILENO);
	close(prev_read);
	if (open_redir_files(cl->redirs) != 0)
		exit(errno);
	signal(SIGINT, sigint_handler);
	execve(cl->args[0], cl->args, env);
}

int	exec_pipe_cmd(t_cmdlst *cl, char **env, t_envlst **el, int *prev_read)
{
	int		pipefd[2];
	pid_t	cpid;

	if (pipe(pipefd) == -1)
		return (minish_errormsg("exec_pipe_cmd", "create pipe failed", errno));
	if (set_exec_path(cl, env) != 0)
		return (ENOENT);
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
	return (0);
}

int	exec_pipe_cmd_last(t_cmdlst *cmdl, char **env, t_envlst **el, int prev_read)
{
	pid_t	cpid;
	char	*exec_path;

	exec_path = get_exec_path(cmdl, env);
	if (exec_path == NULL)
		return (minish_errormsg(cmdl->cmd, "command not found!", ENOENT));
	free(cmdl->args[0]);
	cmdl->args[0] = ft_strdup(exec_path);
	free(exec_path);
	cpid = fork();
	if (cpid < 0)
		return (minish_errormsg("exec_pipe_cmd_last", "fork failed!", errno));
	if (cpid == 0)
	{
		dup2(prev_read, STDIN_FILENO);
		close(prev_read);
		if (open_redir_files(cmdl->redirs))
			exit(errno);
		signal(SIGINT, sigint_handler);
		execve(cmdl->args[0], cmdl->args, env);
	}
	else
		close (prev_read);
	return (0);
}

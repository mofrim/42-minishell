/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_bltin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:08 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/31 00:01:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	run_pipe_bltin(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **));
static int	run_pipe_bltin_last(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **));

int	exec_pipe_bltin(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	int				exit_status;
	t_bltin_pipargs	args;

	exit_status = 0;
	args.cl = cl;
	args.el = el;
	args.prev_read = prev_read;
	if (!ft_strcmp(cl->cmd, "echo"))
		exit_status = run_pipe_bltin(args, NULL, bltin_echo);
	if (!ft_strcmp(cl->cmd, "cd"))
		exit_status = bltin_cd(cl->args, el);
	if (!ft_strcmp(cl->cmd, "pwd"))
		exit_status = run_pipe_bltin(args, NULL, bltin_pwd);
	if (!ft_strcmp(cl->cmd, "export"))
		exit_status = run_pipe_bltin(args, bltin_export_preout, \
					bltin_export_out);
	if (!ft_strcmp(cl->cmd, "unset"))
		exit_status = run_pipe_bltin(args, bltin_unset, NULL);
	if (!ft_strcmp(cl->cmd, "env"))
		exit_status = run_pipe_bltin(args, NULL, bltin_env);
	if (!ft_strcmp(cl->cmd, "exit"))
		exit_status = run_pipe_bltin(args, bltin_exit_preout, bltin_exit_out);
	return (exit_status);
}

static void	run_child(t_bltin_pipargs args, int pipefd[2],
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	dup2(*args.prev_read, STDIN_FILENO);
	close(*args.prev_read);
	if (open_redir_files(args.cl->redirs) != 0)
		exit(errno + 128);
	exit(bltin_out(args.cl, args.el));
}

int	run_pipe_bltin(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	int	cpid;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (minish_errormsg("run_pipe_bltin", "create pipe failed", errno));
	if (bltin_preout)
		bltin_preout(args.cl, args.el);
	if (bltin_out)
	{
		cpid = fork();
		if (cpid < 0)
			return (minish_errormsg("run_pipe_bltin", "fork failed", errno));
		if (cpid == 0)
			run_child(args, pipefd, bltin_out);
		else
		{
			close (pipefd[1]);
			close (*args.prev_read);
			*args.prev_read = pipefd[0];
		}
	}
	return (0);
}

int	exec_pipe_bltin_last(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	int				exit_status;
	t_bltin_pipargs	args;

	exit_status = 0;
	args.cl = cl;
	args.el = el;
	args.prev_read = prev_read;
	if (!ft_strcmp(cl->cmd, "echo"))
		exit_status = run_pipe_bltin_last(args, NULL, bltin_echo);
	if (!ft_strcmp(cl->cmd, "cd"))
		exit_status = bltin_cd(cl->args, el);
	if (!ft_strcmp(cl->cmd, "pwd"))
		exit_status = run_pipe_bltin_last(args, NULL, bltin_pwd);
	if (!ft_strcmp(cl->cmd, "export"))
		exit_status = run_pipe_bltin_last(args, bltin_export_preout, \
					bltin_export_out);
	if (!ft_strcmp(cl->cmd, "unset"))
		exit_status = run_pipe_bltin_last(args, bltin_unset, NULL);
	if (!ft_strcmp(cl->cmd, "env"))
		exit_status = run_pipe_bltin_last(args, NULL, bltin_env);
	if (!ft_strcmp(cl->cmd, "exit"))
		exit_status = run_pipe_bltin_last(args, bltin_exit_preout, \
				bltin_exit_out);
	return (exit_status);
}

int	run_pipe_bltin_last(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	int	cpid;

	if (bltin_preout)
		bltin_preout(args.cl, args.el);
	if (bltin_out)
	{
		cpid = fork();
		if (cpid < 0)
			return (minish_errormsg("run_pipe_bltin", "fork failed", errno));
		if (cpid == 0)
		{
			dup2(*args.prev_read, STDIN_FILENO);
			close(*args.prev_read);
			if (open_redir_files(args.cl->redirs))
				exit(errno);
			exit(bltin_out(args.cl, args.el));
		}
	}
	close (*args.prev_read);
	return (0);
}

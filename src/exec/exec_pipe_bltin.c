/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_bltin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:08 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/11 09:49:38 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The generic bltin execution function for pipelines. Born from the idea to
 * redruce duplicate code. Well. Maybe this is a little bit to generic. But i
 * like it. Only made to be called one time with run_pipe_bltin() and another
 * time with run_pipe_bltin_last(). Due to norminette this is done in
 * exec_pipeline.c */
pid_t	exec_pipe_bltin_generic(t_cmdlst *cl, t_envlst **el, int *prev_read, \
							pid_t (*run_pipe_func)(t_bltin_pipargs, \
								int (*bltin_preout)(t_cmdlst *, t_envlst **), \
								int (*bltin_out)(t_cmdlst *, t_envlst **)))
{
	pid_t			cpid;
	t_bltin_pipargs	args;

	args.cl = cl;
	args.el = el;
	args.prev_read = prev_read;
	if (!ft_strcmp(cl->cmd, "echo"))
		cpid = run_pipe_func(args, NULL, bltin_echo);
	if (!ft_strcmp(cl->cmd, "cd"))
		cpid = run_pipe_func(args, bltin_cd_preout, bltin_cd_out);
	if (!ft_strcmp(cl->cmd, "pwd"))
		cpid = run_pipe_func(args, NULL, bltin_pwd);
	if (!ft_strcmp(cl->cmd, "export"))
		cpid = run_pipe_func(args, bltin_export_preout, \
					bltin_export_out);
	if (!ft_strcmp(cl->cmd, "unset"))
		cpid = run_pipe_func(args, bltin_unset, NULL);
	if (!ft_strcmp(cl->cmd, "env"))
		cpid = run_pipe_func(args, NULL, bltin_env);
	if (!ft_strcmp(cl->cmd, "exit"))
		cpid = run_pipe_func(args, bltin_exit_preout, bltin_exit_out);
	return (cpid);
}

// FIXME: get clarity about the exit codes here! are they correct?
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

// FIXME: get clarity about the exit codes here! are they correct?
// might something bad happen if -1 is returned?
pid_t	run_pipe_bltin(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	pid_t	cpid;
	int		pipefd[2];

	cpid = -1;
	if (pipe(pipefd) == -1)
		return (minish_errormsg("run_pipe_bltin", "create pipe failed", errno));
	if (bltin_preout)
		args.cl->preout_flag = bltin_preout(args.cl, args.el);
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
	return (cpid);
}

// FIXME: get clarity about the exit codes here! are they correct?
// might something bad happen if -1 is returned?
pid_t	run_pipe_bltin_last(t_bltin_pipargs args, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	pid_t	cpid;

	cpid = -1;
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
	return (cpid);
}

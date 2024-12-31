/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:15:39 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/30 23:24:10 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Generalized builtin execution function.
 *
 * Params: cmdlst, envlst, function handling pre-output stuff that changes data
 * which should be available to main process, function handling output stuff
 * which gets executed in a child process with all redirects active.
 */
int	exec_single_builtin(t_cmdlst *cl, t_envlst **el, \
		int (*bltin_preout)(t_cmdlst *, t_envlst **), \
		int (*bltin_out)(t_cmdlst *, t_envlst **))
{
	int	exit_status;
	int	cpid;

	exit_status = 0;
	if (bltin_preout)
		bltin_preout(cl, el);
	if (bltin_out)
	{
		cpid = fork();
		if (cpid == -1)
			return (errno);
		if (cpid == 0)
		{
			if (open_redir_files(cl->redirs))
				exit(errno);
			exit_status = bltin_out(cl, el);
			exit(exit_status);
		}
		waitpid(cpid, &exit_status, 0);
	}
	return (exit_status);
}

/* Finally find the correct builtin function, call it & return their
 * exit_status. */
int	exec_single_builtin_cmd(t_cmdlst *cl, t_envlst **el)
{
	int		exit_status;

	exit_status = 0;
	if (!ft_strcmp(cl->cmd, "echo"))
		exit_status = exec_single_builtin(cl, el, NULL, bltin_echo);
	if (!ft_strcmp(cl->cmd, "cd"))
		exit_status = bltin_cd(cl->args, el);
	if (!ft_strcmp(cl->cmd, "pwd"))
		exit_status = exec_single_builtin(cl, el, NULL, bltin_pwd);
	if (!ft_strcmp(cl->cmd, "export"))
		exit_status = exec_single_builtin(cl, el, bltin_export_preout, \
					bltin_export_out);
	if (!ft_strcmp(cl->cmd, "unset"))
		exit_status = exec_single_builtin(cl, el, bltin_unset, NULL);
	if (!ft_strcmp(cl->cmd, "env"))
		exit_status = exec_single_builtin(cl, el, NULL, bltin_env);
	if (!ft_strcmp(cl->cmd, "exit"))
		exit_status = bltin_exit();
	return (exit_status);
}

/*
 * Execution skeleton for builtins:
 *
 * int	exec_single_builtin_cmd(t_cmdlst *cmdl, t_envlst **el)
 * {
 * 	int	exit_status;
 * 	int	cpid;
 * 
 * 	exit_status = 0;
 * 	cpid = fork();
 * 	if (cpid == -1)
 * 		return (errno);
 * 	if (cpid == 0)
 * 	{
 * 		if (open_redir_files(cmdl->input_file, cmdl->outfiles))
 * 			exit(errno);
 * 		exit(exec_single_builtin(cmdl, el));
 * 	}
 * 	waitpid(cpid, &exit_status, 0);
 * 	return (exit_status >> 8);
 * }
 *
 */

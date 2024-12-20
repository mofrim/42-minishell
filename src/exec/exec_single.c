/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:39:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/20 12:41:25 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_single_simple_cmd(t_cmdlst *cmdl, char **env);

int	exec_single(t_cmdlst *cmdl, char **env, t_envlst **el)
{
	int	exit_status;

	exit_status = 0;
	if (cmdl->is_builtin)
		exit_status = exec_single_builtin_cmd(cmdl, el);
	else if (!cmdl->input_file && !cmdl->outfiles)
		exit_status = exec_single_simple_cmd(cmdl, env);
	else
		exit_status = exec_single_redir_cmd(cmdl, env);
	return (exit_status);
}

int	exec_single_simple_cmd(t_cmdlst *cmdl, char **env)
{
	char	*exec_path;
	int		cpid;
	int		status;

	status = 0;
	exec_path = get_exec_path(cmdl, env);
	if (exec_path == NULL)
	{
		ft_printf("%s: command not found!\n", cmdl->cmd);
		return (ENOENT);
	}
	free(cmdl->args[0]);
	cmdl->args[0] = ft_strdup(exec_path);
	free(exec_path);
	cpid = fork();
	if (cpid < 0)
		error_exit("fork failed");
	if (cpid == 0)
		execve(cmdl->args[0], cmdl->args, env);
	else
		waitpid(cpid, &status, 0);
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 15:22:50 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_simple_cmd(t_cmdlst *cmdl, char **env);

static int	is_simple_cmd(t_cmdlst *cmdl)
{
	return (!cmdl->is_builtin && !cmdl->input_file && !cmdl->output_file && \
			!cmdl->next);
}

int	exec_cmd(t_cmdlst *cmdl, t_envlst *el)
{
	char	**env_arr;
	int		exit_status;

	exit_status = 0;
	env_arr = get_env_array(el);
	if (cmdl->cmd == NULL)
		return (ENOENT);
	if (is_simple_cmd(cmdl))
		exit_status = exec_simple_cmd(cmdl, env_arr);
	free_ptrptr(&env_arr);
	return (exit_status);
}

int	exec_simple_cmd(t_cmdlst *cmdl, char **env)
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
	cmdl->args[0] = exec_path;
	cpid = fork();
	if (cpid < 0)
		error_exit("fork failed");
	if (cpid == 0)
		execve(exec_path, cmdl->args, env);
	else
		waitpid(cpid, &status, 0);
	return (status);
}

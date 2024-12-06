/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eobeng <eobeng@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/07 21:39:03 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* NOTE: this is for now. Maybe later we will have some generic launcher
 * function that can deal with all kinds of commands.
 */
static int exec_simple_cmd(t_cmdlst *cmdl, char **env);
static int is_simple_cmd(t_cmdlst *cmdl);

/* Should be the general execution function. */
int exec_cmd(t_cmdlst *cmdl, t_envlst *el)
{
	char **env_arr;
	int exit_status;

	exit_status = 0;
	env_arr = get_env_array(el);

	if (cmdl->cmd == NULL)
		return (ENOENT);

/***************************************************************************************** */
	// I added these for the purpose of testing. Will refactor if we confirm my functions work	
	if (ft_strcmp(cmdl->cmd, "env") == 0)
	{
		env(el);
		return 0;
	}
	else if (ft_strcmp(cmdl->cmd, "export") == 0)
	{
		export(&el, cmdl->args[1]);
		return 0;
	}
/***************************************************************************************** */

	if (is_simple_cmd(cmdl))
		exit_status = exec_simple_cmd(cmdl, env_arr);
	else if (!cmdl->next)
		exit_status = exec_redir_cmd(cmdl, env_arr);
	else
		exit_status = exec_pipe_cmd(cmdl, env_arr);
	free_ptrptr(&env_arr);
	return (exit_status);
}

static int exec_simple_cmd(t_cmdlst *cmdl, char **env)
{
	char *exec_path;
	int cpid;
	int status;

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

static int is_simple_cmd(t_cmdlst *cmdl)
{
	return (!cmdl->is_builtin && !cmdl->input_file && !cmdl->output_file &&
			!cmdl->next);
}

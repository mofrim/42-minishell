/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:30:55 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 21:46:13 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_single_redir_cmd(t_cmdlst *cmdl, char **env)
{
	char	*exec_path;
	int		cpid;
	int		status;

	exec_path = get_exec_path(cmdl, env);
	if (exec_path == NULL)
		return (print_return_error_msg(cmdl->cmd, \
					"command not found!", ENOENT));
	free(cmdl->args[0]);
	cmdl->args[0] = exec_path;
	cpid = fork();
	if (cpid < 0)
		return (print_return_error_msg("exec_single_redir_cmd", \
					"fork failed!", errno));
	if (cpid == 0)
	{
		if (open_redir_files(cmdl->input_file, cmdl->outfiles, cmdl->append))
			exit(errno);
		execve(exec_path, cmdl->args, NULL);
	}
	waitpid(cpid, &status, 0);
	return (status >> 8);
}

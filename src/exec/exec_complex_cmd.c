/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_complex_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:30:55 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 18:41:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "unistd.h"

// TODO:
// 1) split into seperate functions
// 2) manage pipe chains
int	exec_complex_cmd(t_cmdlst *cmdl, char **env)
{
	int		pipefd[2];
	int		fd;
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
	if (cmdl->next)
	{
		if (pipe(pipefd) == -1)
			error_exit("Pipe creation failed");
		cpid = fork();
		if (cpid < 0)
			error_exit("fork failed");
		if (cpid == 0)
		{
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			if (cmdl->input_file)
			{
				fd = open(cmdl->input_file, O_RDONLY);
				if (fd == -1)
					error_exit("Opening file %s failed");
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			if (cmdl->output_file && !cmdl->append)
			{
				fd = open(cmdl->output_file, O_WRONLY | O_CREAT | O_TRUNC, \
						S_IRUSR | S_IWUSR);
				if (fd == -1)
					error_exit("Opening file %s failed");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			execve(exec_path, cmdl->args, env);
		}
	}
	else
	{
		cpid = fork();
		if (cpid < 0)
			error_exit("fork failed");
		if (cpid == 0)
		{
			if (cmdl->input_file)
			{
				fd = open(cmdl->input_file, O_RDONLY);
				if (fd == -1)
					error_exit("Opening file %s failed");
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			if (cmdl->output_file && !cmdl->append)
			{
				fd = open(cmdl->output_file, O_WRONLY | O_CREAT | O_TRUNC, \
						S_IRUSR | S_IWUSR);
				if (fd == -1)
					error_exit("Opening file %s failed");
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			execve(exec_path, cmdl->args, NULL);
		}
	}
	waitpid(cpid, &status, 0);
	return (status);
}

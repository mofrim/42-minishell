/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_open_redir_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:11:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/08 16:24:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Subroutine for opening possible redirection files for the builtin cmd. */
int	open_redir_files(char *infile, char *outfile, int append)
{
	int	fd;

	if (outfile)
	{
		if (append)
			fd = open(outfile, O_APPEND | O_CREAT | O_WRONLY, \
					S_IRUSR | S_IWUSR);
		else
			fd = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, \
					S_IRUSR | S_IWUSR);
		if (fd == -1)
			return (errno);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (infile)
	{
		fd = open(infile, O_RDONLY);
		if (fd == -1)
			return (errno);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

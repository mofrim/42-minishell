/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_open_redir_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:11:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/08 21:01:46 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Open & create all outfiles like bash does. */
static int	open_create_outfiles(t_heroflst *ofl, int append)
{
	int	fd;

	while (ofl)
	{
		if (append)
			fd = open(ofl->name, O_APPEND | O_CREAT | O_WRONLY, 0600);
		else
			fd = open(ofl->name, O_TRUNC | O_CREAT | O_WRONLY, 0600);
		if (fd == -1)
			return (-1);
		if (ofl->next)
			close(fd);

		ofl = ofl->next;
	}
	return (fd);
}

/* Subroutine for opening possible redirection files for the builtin cmd. */
int	open_redir_files(char *infile, t_heroflst *ofl, int append)
{
	int	fd;

	if (ofl)
	{
		fd = open_create_outfiles(ofl, append);
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

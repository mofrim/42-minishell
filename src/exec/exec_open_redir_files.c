/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_open_redir_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:11:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/20 12:56:17 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirlst(t_redirlst *ofl);
int	proc_redirlst_rout0(t_redirlst *ofl);
int	proc_redirlst_rout1(t_redirlst *ofl);

int	open_redir_files(char *infile, t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	if (ofl)
		rval = process_redirlst(ofl);
	if (infile)
	{
		fd = open(infile, O_RDONLY);
		if (fd == -1)
			return (errno);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (rval);
}

int process_redirlst(t_redirlst *ofl)
{
	int	rval;

	rval = 0;
	while (ofl)
	{
		if (ofl->redtype == RE_ROUT0)
			rval = proc_redirlst_rout0(ofl);
		if (ofl->redtype == RE_ROUT1)
			rval = proc_redirlst_rout1(ofl);
		ofl = ofl->next;
	}
	return (rval);
}

int proc_redirlst_rout0(t_redirlst *ofl)
{
	int	fd;

	fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
		return (errno);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int proc_redirlst_rout1(t_redirlst *ofl)
{
	int	fd;

	fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
		return (errno);
	dup2(fd, ofl->fd_from);
	close(fd);
	return (0);
}

/* Open & create all outfiles like bash does. */
// static int	open_create_outfiles(t_redirlst *ofl, int append)
// {
// 	int	fd;
//
// 	while (ofl)
// 	{
// 		if (append)
// 			fd = open(ofl->name, O_APPEND | O_CREAT | O_WRONLY, 0600);
// 		else
// 			fd = open(ofl->name, O_TRUNC | O_CREAT | O_WRONLY, 0600);
// 		if (fd == -1)
// 			return (-1);
// 		if (ofl->next)
// 			close(fd);
//
// 		ofl = ofl->next;
// 	}
// 	return (fd);
// }

// int	open_redir_files(char *infile, t_redirlst *ofl)
// {
// 	int	fd;
//
// 	if (ofl)
// 	{
// 		// fd = open_create_outfiles(ofl, append);
// 		// if (fd == -1)
// 		// 	return (errno);
// 		// dup2(fd, STDOUT_FILENO);
// 		// close(fd);
// 	}
// 	if (infile)
// 	{
// 		fd = open(infile, O_RDONLY);
// 		if (fd == -1)
// 			return (errno);
// 		dup2(fd, STDIN_FILENO);
// 		close(fd);
// 	}
// 	return (0);
// }

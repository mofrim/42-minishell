/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_open_redir_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:11:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/24 08:11:12 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirlst(t_redirlst *ofl);
int	proc_redirlst_rout0(t_redirlst *ofl);
int	proc_redirlst_rout1(t_redirlst *ofl);
int	proc_redirlst_rout2(t_redirlst *ofl);
int	proc_redirlst_rout3(t_redirlst *ofl);

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

int	process_redirlst(t_redirlst *ofl)
{
	int	rval;

	rval = 0;
	while (ofl)
	{
		if (ofl->redtype == RE_ROUT0)
			rval = proc_redirlst_rout0(ofl);
		else if (ofl->redtype == RE_ROUT1)
			rval = proc_redirlst_rout1(ofl);
		else if (ofl->redtype == RE_ROUT2)
			rval = proc_redirlst_rout2(ofl);
		else if (ofl->redtype == RE_ROUT3)
			rval = proc_redirlst_rout3(ofl);
		if (rval != 0)
			return (rval);
		ofl = ofl->next;
	}
	return (rval);
}

int	proc_redirlst_rout0(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
		return (errno);
	rval = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (rval == -1)
	{
		ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, STDOUT_FILENO);
		return (errno);
	}
	return (0);
}

int	proc_redirlst_rout1(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
		return (errno);
	rval = dup2(fd, ofl->fd_from);
	close(fd);
	if (rval == -1)
	{
		ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, ofl->fd_from);
		return (errno);
	}
	return (0);
}

int	proc_redirlst_rout2(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
		return (errno);
	rval = dup2(fd, STDOUT_FILENO);
	if (rval == -1)
	{
		close(fd);
		ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, STDOUT_FILENO);
		return (errno);
	}
	rval = dup2(fd, STDERR_FILENO);
	close(fd);
	if (rval == -1)
	{
		ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, STDERR_FILENO);
		return (errno);
	}
	return (0);
}

int	proc_redirlst_rout3(t_redirlst *ofl)
{
	int	rval;

	rval = 0;
	if (ofl->fd_from != 0)
		rval = dup2(ofl->fd_to, ofl->fd_from);
	else
		rval = dup2(ofl->fd_to, STDOUT_FILENO);
	if (rval == -1)
	{
		if (errno == 9)
			ft_dprintf(2, "minishell: Baaad bad file descriptor %d\n", \
					ofl->fd_to);
		return (errno);
	}
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

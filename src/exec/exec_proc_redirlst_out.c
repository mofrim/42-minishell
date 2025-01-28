/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_proc_redirlst_out.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 08:17:00 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 23:36:06 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	proc_redirlst_rout0(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	if (ofl->redtype == RE_ROUT0)
		fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else
		fd = open(ofl->outfile, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		return (minish_errormsg(ofl->outfile, strerror(errno), errno));
	rval = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (rval == -1)
		return (ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, \
					STDOUT_FILENO), errno);
	return (0);
}

int	proc_redirlst_rout1(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	if (ofl->redtype == RE_ROUT1)
		fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else
		fd = open(ofl->outfile, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (errno);
	rval = dup2(fd, ofl->fd_from);
	close(fd);
	if (rval == -1)
		return (ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, \
					ofl->fd_from), errno);
	return (0);
}

int	proc_redirlst_rout2(t_redirlst *ofl)
{
	int	fd;
	int	rval;

	rval = 0;
	if (ofl->redtype == RE_ROUT2)
		fd = open(ofl->outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else
		fd = open(ofl->outfile, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd == -1)
		return (errno);
	rval = dup2(fd, STDOUT_FILENO);
	if (rval == -1)
		return (close(fd), ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", \
					fd, STDOUT_FILENO), errno);
	rval = dup2(fd, STDERR_FILENO);
	close(fd);
	if (rval == -1)
		return (ft_dprintf(2, "minishell: dup2(%d, %d) failed\n", fd, \
					STDERR_FILENO), errno);
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

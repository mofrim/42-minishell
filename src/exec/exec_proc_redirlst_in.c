/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_proc_redirlst_in.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:18:35 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 18:30:01 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "unistd.h"

/* Process `<file` and `n<file` redirs. */
int	proc_redirlst_rin01(t_redirlst *rdl)
{
	int	fd;
	int	rval;

	rval = 0;
	fd = open(rdl->infile, O_RDONLY);
	if (fd == -1)
		return (ft_dprintf(2, "minishell: %s: %s\n", rdl->infile, \
					strerror(errno)), errno);
	if (rdl->redtype == RE_RIN1)
		rval = dup2(fd, rdl->fd_to);
	else
		rval = dup2(fd, STDIN_FILENO);
	close(fd);
	if (rval == -1)
		return (ft_dprintf(2, "minishell: dup2 failed\n"), errno);
	return (0);
}

/**
 * Process the `<>` redir operator.
 *
 * From the man:
 *
 * The redirection operator
 * 
 *               [n]<>word
 * 
 * causes the file whose name is the expansion of word to be opened for
 * both reading and writing on file descriptor n, or on file descriptor 0
 * if n is not specified.  If the  file  does not exist, it is created.
 *
 * NOTE: rdl->fd_to == 0 by default anyway. So in case of TOK_RINOUT0 there is
 * no problem.
 */
int	proc_redirlst_rinout(t_redirlst *rdl)
{
	int	fd;
	int	rval;

	rval = 0;
	fd = open(rdl->infile, O_CREAT | O_RDWR, 0600);
	if (fd == -1)
		return (ft_dprintf(2, "minishell: %s: %s\n", rdl->infile, \
					strerror(errno)), errno);
	rval = dup2(fd, rdl->fd_to);
	close(fd);
	if (rval == -1)
		return (ft_dprintf(2, "minishell: dup2 failed\n"), errno);
	return (0);
}

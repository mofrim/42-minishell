/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_open_redir_files.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 16:11:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 18:34:00 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_redirlst(t_redirlst *rdl);
int	proc_redirlst_rout0(t_redirlst *rdl);
int	proc_redirlst_rout1(t_redirlst *rdl);
int	proc_redirlst_rout2(t_redirlst *rdl);
int	proc_redirlst_rout3(t_redirlst *rdl);
int	proc_redirlst_rin01(t_redirlst *rdl);
int	proc_redirlst_rinout(t_redirlst *rdl);

int	open_redir_files(t_redirlst *rdl)
{
	int	rval;

	rval = 0;
	if (rdl)
		rval = process_redirlst(rdl);
	return (rval);
}

int	process_redirlst(t_redirlst *rdl)
{
	int	rval;

	rval = 0;
	while (rdl)
	{
		if (rdl->redtype == RE_ROUT0 || rdl->redtype == RE_ROUTA0)
			rval = proc_redirlst_rout0(rdl);
		else if (rdl->redtype == RE_ROUT1 || rdl->redtype == RE_ROUTA1)
			rval = proc_redirlst_rout1(rdl);
		else if (rdl->redtype == RE_ROUT2 || rdl->redtype == RE_ROUTA2)
			rval = proc_redirlst_rout2(rdl);
		else if (rdl->redtype == RE_ROUT3)
			rval = proc_redirlst_rout3(rdl);
		else if (rdl->redtype == RE_RIN0 || rdl->redtype == RE_RIN1)
			rval = proc_redirlst_rin01(rdl);
		else if (rdl->redtype == RE_RINOUT)
			rval = proc_redirlst_rinout(rdl);
		if (rval != 0)
			return (rval);
		rdl = rdl->next;
	}
	return (rval);
}

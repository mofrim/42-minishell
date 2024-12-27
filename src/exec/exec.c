/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 23:46:17 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The general execution function. Two cases possible: with pipes or without.
 * Dispatches correspondingly to exec_pipe() or exec_single().
 * Params: cmdls and envlst. */
int	exec_cmd(t_cmdlst *cmdl, t_envlst **el)
{
	char	**env_arr;
	int		exit_status;

	exit_status = 0;
	if (cmdl->cmd == NULL)
		return (ENOENT);
	env_arr = get_env_array(*el);
	nullcheck(env_arr, "exec_cmd()");
	if (cmdl->next)
		exit_status = exec_pipeline(cmdl, env_arr, el);
	else
		exit_status = exec_single(cmdl, env_arr, el);
	free_ptrptr(&env_arr);
	return (exit_status);
}


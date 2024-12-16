/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 10:25:30 by elpah            ###   ########.fr       */
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
	env_arr = get_env_array(*el);
	if (cmdl->cmd == NULL)
		return (ENOENT);
	/* if i uncomment this then the export works fine. other than that, it is only able to print and nothing more.*/
	// if (!strcmp(cmdl->cmd, "export"))
		// exit_status= bltin_export(el, cmdl->args[1]);
	if (cmdl->next)
		exit_status = exec_pipe(cmdl, env_arr, el);
	else
		exit_status = exec_single(cmdl, env_arr, el);
	free_ptrptr(&env_arr);
	return (exit_status);
}


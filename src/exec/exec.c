/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:43:14 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/02 20:06:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * The general execution function.
 *
 * Two cases possible: with pipes or without. Dispatches correspondingly to
 * `exec_pipeline` or `exec_single`. We block our minishell-signal-handling
 * before starting to launch any subprocess so that it does not get in the way
 * of any subprocesses signal handling. After everything is done our
 * minishell-signal-handling continues. */
int	exec_cmd(t_cmdlst *cmdl, t_envlst **el)
{
	char	**env_arr;
	int		exit_status;

	exit_status = 0;
	if (cmdl->cmd == NULL && cmdl->redirs == NULL)
		return (ENOENT);
	env_arr = get_env_array(*el);
	nullcheck(env_arr, "exec_cmd()");
	signal(SIGINT, SIG_IGN);
	if (cmdl->next)
		exit_status = exec_pipeline(cmdl, env_arr, el);
	else
		exit_status = exec_single(cmdl, env_arr, el);
	signal(SIGINT, minish_sighandler);
	free_ptr2ptr(&env_arr);
	return (exit_status);
}

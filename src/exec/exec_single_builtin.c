/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:15:39 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 13:26:36 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Execute the single builtin command. Also opens possible redirection files
 * beforehand.
 *
 * We are not allowed to use WIFEXITED and WEXITSTATUS as they are macros. But
 * the calculation needed for extracting the real exit status from that recorded
 * by waitpid is simply shifting the bits to the right by 8. */

// This is the skeleton for executing a builtin. Copy this to the bltin_*.c file
// and split the original function into bltin_*_out() and bltin_*_preout(). Take
// export as an example!
//
// The skeleton:

// int	exec_single_builtin(t_cmdlst *cmdl, t_envlst **el)
// {
// 	int	exit_status;
// 	int	cpid;
//
// 	exit_status = 0;
// 	cpid = fork();
// 	if (cpid == -1)
// 		return (errno);
// 	if (cpid == 0)
// 	{
// 		if (open_redir_files(cmdl->input_file, cmdl->outfiles, cmdl->append))
// 			exit(errno);
// 		exit(exec_single_builtin(cmdl, el));
// 	}
// 	waitpid(cpid, &exit_status, 0);
// 	return (exit_status >> 8);
// }

/* Finally find the correct builtin function, call it & return their
 * exit_status. */
int	exec_single_builtin_cmd(t_cmdlst *cl, t_envlst **el)
{
	int		exit_status;

	exit_status = 0;
	if (!strcmp(cl->cmd, "echo"))
		exit_status = bltin_echo(cl->args);
	if (!strcmp(cl->cmd, "cd"))
		exit_status = bltin_cd(cl->args, el);
	if (!strcmp(cl->cmd, "pwd"))
		exit_status = bltin_pwd();
	if (!strcmp(cl->cmd, "export"))
		exit_status = bltin_export(cl, el);
	if (!strcmp(cl->cmd, "unset"))
		exit_status = bltin_unset(el, cl->args);
	if (!strcmp(cl->cmd, "env"))
		exit_status = bltin_env(cl, *el);
	if (!strcmp(cl->cmd, "exit"))
		exit_status = bltin_exit();
	return (exit_status);
}

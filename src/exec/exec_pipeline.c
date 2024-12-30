/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:07:34 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/30 11:03:03 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pipe_cmd(t_cmdlst *cl, char **env, int *prev_read);
int	exec_pipe_cmd_last(t_cmdlst *cl, char **env, int prev_read);
int	exec_pipe_redir_cmd(t_cmdlst *cl, char **env);
int	exec_pipe_bltin(t_cmdlst *cl, t_envlst **el, int *prev_read);
int	exec_pipe_bltin_last(t_cmdlst *cl, t_envlst **el, int *prev_read);

int	exec_pipeline(t_cmdlst *cl, char **env, t_envlst **el)
{
	int prev_read;
	int	status;

	prev_read = dup(0);
	status = 0;
	signal(SIGINT, SIG_IGN);
	while (cl->next)
	{
		if (cl->is_builtin)
			exec_pipe_bltin(cl, el, &prev_read);
		else
			exec_pipe_cmd(cl, env, &prev_read);
		cl = cl->next;
	}
	if (cl->is_builtin)
		exec_pipe_bltin_last(cl, el, &prev_read);
	else
		exec_pipe_cmd_last(cl, env, prev_read);
	while (!(wait(&status) == -1 && errno == ECHILD))
		;
	signal(SIGINT, sigint_handler);
	return (status);
}

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

static pid_t	exec_pipe_bltin(t_cmdlst *cl, t_envlst **el, int *prev_read);
static pid_t	exec_pipe_bltin_last(t_cmdlst *cl, t_envlst **el, \
										int *prev_read);
static void		wait_for_each_child(pid_t *pids, int pidindx, int *status);
pid_t			exec_pipe_cmd(t_cmdlst *cl, char **env, int *prev_read);
pid_t			exec_pipe_cmd_last(t_cmdlst *cl, char **env, int prev_read);
pid_t			exec_pipe_bltin_generic(t_cmdlst *cl, t_envlst **el, \
					int *prev_read, pid_t (*run_pipe_func)(t_bltin_pipargs, \
							int (*bltin_preout)(t_cmdlst *, t_envlst **), \
							int (*bltin_out)(t_cmdlst *, t_envlst **)));
pid_t			run_pipe_bltin_last(t_bltin_pipargs args, \
							int (*bltin_preout)(t_cmdlst *, t_envlst **), \
							int (*bltin_out)(t_cmdlst *, t_envlst **));
pid_t			run_pipe_bltin(t_bltin_pipargs args, \
						int (*bltin_preout)(t_cmdlst *, t_envlst **), \
						int (*bltin_out)(t_cmdlst *, t_envlst **));

int	exec_pipeline(t_cmdlst *cl, char **env, t_envlst **el)
{
	int		prev_read;
	int		status;
	pid_t	*pids;
	int		pidindx;

	pids = malloc(sizeof(pid_t) * cl->cmd_count);
	prev_read = dup(0);
	status = 0;
	pidindx = 0;
	while (cl->next)
	{
		if (cl->is_builtin)
			pids[pidindx++] = exec_pipe_bltin(cl, el, &prev_read);
		else
			pids[pidindx++] = exec_pipe_cmd(cl, env, &prev_read);
		cl = cl->next;
	}
	if (cl->is_builtin)
		pids[pidindx++] = exec_pipe_bltin_last(cl, el, &prev_read);
	else
		pids[pidindx++] = exec_pipe_cmd_last(cl, env, prev_read);
	wait_for_each_child(pids, pidindx, &status);
	while (!(wait(&status) == -1 && errno == ECHILD))
		;
	return (free(pids), status);
}

static void	wait_for_each_child(pid_t *pids, int pidindx, int *status)
{
	int	i;

	i = -1;
	while (++i < pidindx)
		waitpid(pids[i], status, 0);
}

pid_t	exec_pipe_bltin(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	return (exec_pipe_bltin_generic(cl, el, prev_read, run_pipe_bltin));
}

pid_t	exec_pipe_bltin_last(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	return (exec_pipe_bltin_generic(cl, el, prev_read, run_pipe_bltin_last));
}

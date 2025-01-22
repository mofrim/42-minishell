/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 12:07:34 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/22 09:08:34 by fmaurer          ###   ########.fr       */
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

/**
 * The main pipeline execution function.
 *
 * Execute all pipelined cmds. Make the `prev_read` var a duplicate of stdin.
 * This wouldn't be necessary for the first pipe cmd, but in the code of the
 * `exec_pipe_cmd` and `exec_pipe_bltin` functions stdin is always
 * duplicated to `prev_read` which then is the read-end of the pipes.
 * In the end we wait for alle child processes in order to catch the exit-status
 * of the last pipe cmd.
 */
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
	return (free(pids), status);
}

/* Wait for all child processes in an orderly fashion. Meaning: we want the
 * `status` variable to really hold the exit-status of the last command executed
 * in the pipeline. */
static void	wait_for_each_child(pid_t *pids, int pidindx, int *status)
{
	int	i;

	i = -1;
	while (++i < pidindx)
		waitpid(pids[i], status, 0);
}

/* Wrapper for executing buitlin in a pipe using our generic function. */
pid_t	exec_pipe_bltin(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	return (exec_pipe_bltin_generic(cl, el, prev_read, run_pipe_bltin));
}

/* Wrapper for executing buitlin as the last command in a pipe using our generic
 * function. */
pid_t	exec_pipe_bltin_last(t_cmdlst *cl, t_envlst **el, int *prev_read)
{
	return (exec_pipe_bltin_generic(cl, el, prev_read, run_pipe_bltin_last));
}

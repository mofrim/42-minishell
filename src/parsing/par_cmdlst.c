/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/03 22:30:55 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Init new cmdlst-struct. Init arg_count at 1 because we want cmd->args[0] to
 * be free for the final exec_path. 
 * Why maxargs+2? Because we always want to have at least a arg-array which is
 * able to store cmd as argv[0] and a NULL termination.
 *
 * Making this a bidirectional linked list in order to be able to find the head
 * of list from some other list member for setting the exit_flag in exit_bltin.
 * */
t_cmdlst	*cmdlst_new(char *exec, int maxargs)
{
	t_cmdlst	*cl;

	cl = malloc(sizeof(t_cmdlst));
	nullcheck(cl, "cmdlst_new()");
	if (!cl)
		return (NULL);
	cl->cmd = ft_strdup(exec);
	cl->args = malloc(sizeof(char *) * (maxargs + 2));
	cl->arg_count = 1;
	cl->cmd_count = 1;
	cl->is_builtin = 0;
	cl->exit_flag = 0;
	cl->heredoc = NULL;
	cl->next = NULL;
	cl->prev = NULL;
	cl->redirs = NULL;
	return (cl);
}

void	cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend)
{
	t_cmdlst	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = cmdlst_last(*head);
	oldend->next = newend;
	newend->prev = oldend;
}

/* Free args-split from cmdlst-struct. If argcnt == 1, only args[0] which is the
 * cmd will be freed. Imn this case argv[1] is holding NULL. Everything fine.
 * If argcnt == n , then until argv[n-1] everything will be freed argv[n] will
 * hold NULL. Everything LGTM.*/
void	free_args(char ***args, int argcnt)
{
	int	i;

	if (argcnt >= 1)
	{
		i = -1;
		while (++i < argcnt)
			free((*args)[i]);
		free (*args);
		return ;
	}
	if (**args)
		free (*args);
}

void	cmdlst_clear(t_cmdlst **lst)
{
	t_cmdlst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->cmd)
			free((*lst)->cmd);
		free_args(&(*lst)->args, (*lst)->arg_count);
		if ((*lst)->redirs)
			redirlst_clear(&(*lst)->redirs);
		if ((*lst)->heredoc)
			free((*lst)->heredoc);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

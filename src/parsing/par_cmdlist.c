/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 16:14:56 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Init new cmdlst-struct. Init arg_count at 1 because we want cmd->args[0] to
 * be free for the final exec_path. 
 * Why maxargs+2? Because we always want to have at least a arg-array which is
 * able to store cmd as argv[0] and a NULL termination.
 *
 * */
t_cmdlst	*cmdlst_new(char *exec, int maxargs)
{
	t_cmdlst	*cmd;

	cmd = malloc(sizeof(t_cmdlst));
	nullcheck(cmd, "cmdlst_new()");
	if (!cmd)
		return (NULL);
	cmd->cmd = ft_strdup(exec);
	cmd->args = malloc(sizeof(char *) * (maxargs + 2));
	cmd->arg_count = 1;
	cmd->is_builtin = 0;
	cmd->append = 0;
	cmd->exit_flag = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

t_cmdlst	*cmdlst_last(t_cmdlst *head)
{
	t_cmdlst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
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

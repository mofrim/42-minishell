/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 15:27:59 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Init new cmdlst-struct. Init arg_count at 1 because we want cmd->args[0] to
 * be free for the final exec_path. */
// FIXME: remove args limit which could definitely lead to overflows!!!
// FIXME: think about the cmd / args[0] thing. this is a little bit to
// confusing.
t_cmdlst	*cmdlst_new(char *exec)
{
	t_cmdlst	*cmd;

	cmd = malloc(sizeof(t_cmdlst));
	nullcheck(cmd, "cmdlst_new()");
	if (!cmd)
		return (NULL);
	cmd->cmd = ft_strdup(exec);
	cmd->args = malloc(sizeof(char *) * 10);
	cmd->arg_count = 1;
	cmd->is_builtin = 0;
	cmd->append = 0;
	cmd->heredoc = NULL;
	cmd->next = NULL;
	cmd->output_file = NULL;
	cmd->input_file = NULL;
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

/* Free args-split from cmdlst-struct. */
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

/*
 * NOTE: no need to free cmdlst->cmd because args[0] is storing the same
 * pointer.
 */
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
		if ((*lst)->output_file)
			free((*lst)->output_file);
		if ((*lst)->input_file)
			free((*lst)->input_file);
		if ((*lst)->heredoc)
			free((*lst)->heredoc);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/03 11:03:53 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlst	*cmdlst_new(char *exec)
{
	t_cmdlst	*cmd;

	cmd = malloc(sizeof(t_cmdlst));
	nullcheck(cmd, "cmdlst_new()");
	if (!cmd)
		return (NULL);
	cmd->executable = ft_strdup(exec);
	cmd->args = malloc(sizeof(char *) * 10);
	cmd->arg_count = 0;
	cmd->is_builtin = 0;
	cmd->append = 0;
	cmd->heredoc = 0;
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

	if (argcnt > 0)
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
		free((*lst)->executable);
		free_args(&(*lst)->args, (*lst)->arg_count);
		if ((*lst)->output_file)
			free((*lst)->output_file);
		if ((*lst)->input_file)
			free((*lst)->input_file);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

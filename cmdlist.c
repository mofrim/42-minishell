/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 22:55:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlst	*cmdlst_new(char *exec)
{
	t_cmdlst	*cmd;

	// TODO handle malloc
	cmd = malloc(sizeof(t_cmdlst));
	if (!cmd)
		return (NULL);
	cmd->executable = ft_strdup(exec);
	cmd->args = malloc(sizeof(char *) * 10);
	cmd->arg_count = 0;
	cmd->next = NULL;
	cmd->output_file = NULL;
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

void	cmdlst_clear(t_cmdlst **lst)
{
	t_cmdlst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->executable);

		// FIXME needs to be free_doubleptr()!
		free((*lst)->args);

		free((*lst)->output_file);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	cmdlst_size(t_cmdlst *lst)
{
	int	cnt;

	cnt = 0;
	if (!lst)
		return (0);
	while (lst)
	{
		cnt++;
		lst = lst->next;
	}
	return (cnt);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_envlst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/02 18:49:03 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envlst	*envlst_new(char *name, char *val)
{
	t_envlst	*nn;

	nn = (t_envlst *) malloc(sizeof(t_envlst));
	if (!nn)
		return (NULL);
	nn->name = name;
	nn->value = val;
	nn->next = NULL;
	return (nn);
}

t_envlst	*envlst_last(t_envlst *head)
{
	t_envlst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	envlst_add_back(t_envlst **head, t_envlst *newend)
{
	t_envlst	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = envlst_last(*head);
	oldend->next = newend;
}

void	envlst_clear(t_envlst **lst)
{
	t_envlst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->name);
		free((*lst)->value);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	envlst_size(t_envlst *lst)
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

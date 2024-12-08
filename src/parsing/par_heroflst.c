/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_heroflst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 19:26:24 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/08 19:27:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Create new heroflst entry. String duplication is handled *inside* this
 * function! */
t_heroflst	*heroflst_new(char *name, char *val)
{
	t_heroflst	*nn;

	nn = (t_heroflst *) malloc(sizeof(t_heroflst));
	if (!nn)
		return (NULL);
	nn->name = ft_strdup(name);
	nn->next = NULL;
	return (nn);
}

t_heroflst	*heroflst_last(t_heroflst *head)
{
	t_heroflst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	heroflst_add_back(t_heroflst **head, t_heroflst *newend)
{
	t_heroflst	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = heroflst_last(*head);
	oldend->next = newend;
}

void	heroflst_clear(t_heroflst **lst)
{
	t_heroflst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->name);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	heroflst_size(t_heroflst *lst)
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

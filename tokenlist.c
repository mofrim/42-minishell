/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenlist.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 17:41:37 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenlist	*toklst_new(t_token *tok)
{
	t_tokenlist	*nn;

	nn = (t_tokenlist *) malloc(sizeof(t_tokenlist));
	if (!nn)
		return (NULL);
	nn->token = tok;
	nn->next = NULL;
	return (nn);
}

t_tokenlist	*toklst_last(t_tokenlist *head)
{
	t_tokenlist	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	toklst_add_back(t_tokenlist **head, t_tokenlist *newend)
{
	t_tokenlist	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = toklst_last(*head);
	oldend->next = newend;
}

void	toklst_clear(t_tokenlist **lst)
{
	t_tokenlist	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->token);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	toklst_size(t_tokenlist *lst)
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

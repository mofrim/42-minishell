/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenlist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 13:31:29 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_toklst	*toklst_new(t_token *tok)
{
	t_toklst	*nn;

	nn = (t_toklst *) malloc(sizeof(t_toklst));
	if (!nn)
		return (NULL);
	nn->token = tok;
	nn->next = NULL;
	return (nn);
}

t_toklst	*toklst_last(t_toklst *head)
{
	t_toklst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	toklst_add_back(t_toklst **head, t_toklst *newend)
{
	t_toklst	*oldend;

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

void	toklst_clear(t_toklst **lst)
{
	t_toklst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->token->value);
		free((*lst)->token);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

int	toklst_size(t_toklst *lst)
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

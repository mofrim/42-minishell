/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_toklst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:22:51 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 22:08:48 by fmaurer          ###   ########.fr       */
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
	nn->prev = NULL;
	return (nn);
}

/**
 * Create a new toklst struct by specifying toktype and value.
 *
 * A shortcut function for initializing toklst members more quickly.
 */
t_toklst	*toklst_new_tok(t_toktype type, char *value)
{
	t_toklst	*nn;
	t_token		*tok;

	nn = (t_toklst *) malloc(sizeof(t_toklst));
	if (!nn)
		return (NULL);
	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(value);
	tok->type = type;
	nn->token = tok;
	nn->next = NULL;
	nn->prev = NULL;
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
	newend->prev = oldend;
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_herdlst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 19:26:24 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/14 22:02:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Create new herdlst entry. String duplication is handled *inside* this
 * function! */
t_herdlst	*herdlst_new(t_token *tok)
{
	t_herdlst	*nn;

	nn = (t_herdlst *) malloc(sizeof(t_herdlst));
	if (!nn)
		return (NULL);
	nn->name = ft_strdup(tok->value);
	nn->type = tok->type;
	nn->next = NULL;
	return (nn);
}

t_herdlst	*herdlst_last(t_herdlst *head)
{
	t_herdlst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	herdlst_add_back(t_herdlst **head, t_herdlst *newend)
{
	t_herdlst	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = herdlst_last(*head);
	oldend->next = newend;
}

void	herdlst_clear(t_herdlst **lst)
{
	t_herdlst	*tmp;

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

void	herdlst_print(t_herdlst *lst)
{
	while (lst)
	{
		ft_printf("%s ", lst->name);
		lst = lst->next;
	}
}

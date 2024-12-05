/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenlist_del.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:17:51 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 13:14:40 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_one(t_toklst *lst)
{
	free(lst->token->value);
	free(lst->token);
	free(lst);
}

/* Delete one toklst member from a tokenlist. */
void	toklst_del(t_toklst **lst, t_toklst *delme)
{
	t_toklst	*prev;
	t_toklst	*next;
	t_toklst	*tmp;

	tmp = *lst;
	if (*lst && delme && *lst == delme)
	{
		next = (*lst)->next;
		free_one(tmp);
		*lst = next;
		return ;
	}
	while (tmp)
	{
		if (tmp->next == delme)
			prev = tmp;
		if (tmp == delme)
		{
			next = tmp->next;
			free_one(tmp);
			prev->next = next;
			return ;
		}
		tmp = tmp->next;
	}
}

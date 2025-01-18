/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenlist_del.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:17:51 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/19 00:48:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_one_toklst(t_toklst *lst)
{
	free(lst->token->value);
	free(lst->token);
	free(lst);
}

static void	del_first_toklst_member(t_toklst **lst)
{
	t_toklst	*next;

	next = (*lst)->next;
	free_one_toklst(*lst);
	*lst = next;
	if (next)
		next->prev = NULL;
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
		del_first_toklst_member(lst);
		return ;
	}
	while (tmp)
	{
		if (tmp->next == delme)
			prev = tmp;
		if (tmp == delme)
		{
			next = tmp->next;
			free_one_toklst(tmp);
			prev->next = next;
			if (next)
				next->prev = prev;
			return ;
		}
		tmp = tmp->next;
	}
}

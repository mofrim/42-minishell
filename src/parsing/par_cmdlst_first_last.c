/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlst_first_last.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 22:21:42 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/03 22:23:46 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Get last list member. */
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

/* Get first member of list aka head. */
t_cmdlst	*cmdlst_head(t_cmdlst *cur)
{
	t_cmdlst	*head;

	if (!cur)
		return (NULL);
	head = cur;
	while (head->prev != NULL)
		head = head->prev;
	return (head);
}

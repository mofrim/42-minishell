/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_envlst_delone.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:08:27 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 19:42:30 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Del one envlst member from envlst el.
 *
 * Returns 0 if "name" could be removed from envlst, -1 otherwise.
 *
 */
int	envlst_delone_by_name(char *name, t_envlst **el)
{
	t_envlst *delme;

	delme = get_env_entry_by_name(name, *el);
	if (delme == NULL)
		return (-1);
	if (delme->prev == NULL)
	{
		*el = delme->next;
		free(delme->name);
		free(delme->value);
		free(delme);
	}
	else
	{
		delme->prev->next = delme->next;
		if (delme->next)
			delme->next->prev = delme->prev;
		free(delme->name);
		free(delme->value);
		free(delme);
	}
	return (0);
}

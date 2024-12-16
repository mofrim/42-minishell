/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_entry.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:11 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 12:35:30 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Takes two strings and sets the iven env-var to their value. String
 * duplication is handled *inside* this function! */
void set_env_entry(char *name, char *value, t_envlst **el)
{
	t_envlst *entry;

	entry = get_env_entry_by_name(name, *el);
	if (entry)
	{
		if (value)
		{
			if (entry->value)
				free(entry->value);
			entry->value = ft_strdup(value);
		}
		return ;
	}
	entry = envlst_new(name, value);
	envlst_add_back(el, entry);
}

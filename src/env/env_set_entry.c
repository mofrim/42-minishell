/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_entry.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:11 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 14:47:20 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Set the env var `name` to value `value`.
 *
 * If a env var with named `name` already exists free the value and strdups the
 * new value to it.
 * If the env var did not exist create it with name, value.
 */
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

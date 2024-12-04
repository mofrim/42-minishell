/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_entry.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:45:11 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 16:10:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_entry(char *name, char *value, t_envlst **el)
{
	t_envlst	*entry;

	entry = get_env_entry_by_name(name, *el);
	if (entry)
	{
		if (entry->value)
			free(entry->value);
		entry->value = ft_strdup(value);
		return ;
	}
	entry = envlst_new(name, value);
	envlst_add_back(el, entry);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 04:16:18 by elpah             #+#    #+#             */
/*   Updated: 2024/12/30 04:16:35 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_env_value(t_envlst *lst, char *name, char *new_value)
{
	t_envlst *ptr;

	ptr = lst;
	while (ptr != NULL)
	{
		if (!ft_strcmp(ptr->name, name))
		{
			free(ptr->value);
			ptr->value = ft_strdup(new_value);
			return;
		}
		ptr = ptr->next;
	}
}
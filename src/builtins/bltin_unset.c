/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:14:32 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 10:37:45 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void unset_free(t_envlst *ptr)
{
	if (!ptr)
		return;
	free(ptr->name);
	free(ptr->value);
	free(ptr);
}

int check(char **key)
{
	if (!key[1])
	{
		ft_printf("-minishell: unset: not enough arguments\n");
		return (0);
	}
	if (key[1][0] == '=' || key[1][0] == '?' || key[1][0] == '-')
	{
		ft_printf("-minishell: unset: '%s': not a valid identifier\n", key[1]);
		return (0);
	}
	return (1);
}

int bltin_unset(t_envlst **env, char **str)
{
	t_envlst *ptr;
	t_envlst *prev;
	t_envlst *ptr2;

	if (!env || !*env || !check(str))
		return (0);

	ptr = *env;
	if (!ft_strcmp(ptr->name, str[1]))
	{
		*env = ptr->next;
		unset_free(ptr);
		return (0);
	}
	prev = ptr;
	while (ptr && ptr->next)
	{
		if (!ft_strcmp(ptr->next->name, str[1]))
		{
			ptr2 = ptr->next;
			ptr->next = ptr->next->next;
			unset_free(ptr2);
			return (0);
		}
		ptr = ptr->next;
	}
	return 		(0);
}

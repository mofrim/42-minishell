/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 03:45:25 by elpah             #+#    #+#             */
/*   Updated: 2024/12/16 08:22:46 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envlst	*envlst_copy(t_envlst *env)
{
	t_envlst	*copy;
	t_envlst	*new_node;

	copy = NULL;
	while (env)
	{
		new_node = envlst_new(env->name, env->value);
		envlst_add_back(&copy, new_node);
		env = env->next;
	}
	return (copy);
}

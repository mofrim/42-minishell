/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eobeng <eobeng@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 20:56:01 by eobeng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Export env-var with name and value to global envlst. Return 0 if everything
 * went fine. Return -1 if f.ex. name was not correctly formatted. */
// TODO implement error checking

// Export without any arguments prints a sorted env.
t_envlst *envlst_copy(t_envlst *env)
{
	t_envlst *copy = NULL;
	t_envlst *new_node;

	while (env)
	{
		new_node = envlst_new(env->name, env->value);
		envlst_add_back(&copy, new_node); // Add to the copy list
		env = env->next;
	}
	return copy;
}

// Swap Env
void swap_env_vars(t_envlst *a, t_envlst *b)
{
	char *temp_name = a->name;
	char *temp_value = a->value;

	a->name = b->name;
	a->value = b->value;

	b->name = temp_name;
	b->value = temp_value;
}
// Sort env
void sort_env_list(t_envlst *env)
{
	t_envlst *i;
	t_envlst *j;

	i = env;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (ft_strcmp(i->name, j->name) > 0)
			{
				swap_env_vars(i, j);
			}
			j = j->next;
		}
		i = i->next;
	}
}
int export(t_envlst **env, char *arg)
{
	char *name;
	char *value;
	char **str;

	t_envlst *newvar;
	t_envlst *env_copy;

	// Assuming no argument was added and i only called export
	if (arg == NULL)
	{
		env_copy = envlst_copy(*env);
		sort_env_list(env_copy);

		while (env_copy)
		{
			if (env_copy->value)
				printf("declare -x %s=\"%s\"\n", env_copy->name, env_copy->value);
			// here i am printing differently to match bash output
			else
				printf("declare -x %s\n", env_copy->name);
			env_copy = env_copy->next;
		}
		envlst_clear(&env_copy);
		return 0;
	}
	str = ft_split(arg, '=');
	if (str && str[0])
	{
		name = str[0];
		if (str[1])
			value = str[1];
		else
			value = NULL;
		newvar = envlst_new(name, value);
		envlst_add_back(env, newvar);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
		newvar = envlst_new(name, value);
		envlst_add_back(env, newvar);
	}

	free(str);
	return (0);
}

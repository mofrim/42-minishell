/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/05 05:01:59 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Export env-var with name and value to global envlst. Return 0 if everything
 * went fine. Return -1 if f.ex. name was not correctly formatted. */
// TODO implement error checking

// FIXME: export bla="yadayada" should work as expected, i.e. set the env-var
// `zzz` to the string yadayada, without the quotes.

// Swap Env
void	swap_env_vars(t_envlst *a, t_envlst *b)
{
	char	*temp_name;
	char	*temp_value;

	temp_name = a->name;
	temp_value = a->value;
	a->name = b->name;
	a->value = b->value;
	b->name = temp_name;
	b->value = temp_value;
}
// Sort env

void	sort_env_list(t_envlst *env)
{
	t_envlst	*i;
	t_envlst	*j;

	i = env;
	while (i != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (ft_strcmp(i->name, j->name) > 0)
				swap_env_vars(i, j);
			j = j->next;
		}
		i = i->next;
	}
}

int	bltin_export_preout(t_cmdlst *cl, t_envlst **el)
{
	char	*name;
	char	*value;
	char	**str;
	int		i;
	int		invalid_found;

	invalid_found = 0;
	if (cl->args[1] == NULL)
		return (0);
	invalid_found = check_valid_vars(cl->args);
	i = 0;
	while (cl->args[++i] != NULL)
	{
		value = NULL;
		str = ft_split_input(cl->args[i]);
		if (str && str[0])
		{
			name = str[0];
			if (str[1] && str[1][0])
				value = str[1];
			set_env_entry(name, value, el);
		}
		free(str);
	}
	return (invalid_found);
}

int	bltin_export_out(t_cmdlst *cl, t_envlst **el)
{
	if (cl->preout_flag)
		return (1);
	if (cl->args[1] == NULL)
		print_exported_variables(*el);
	return (0);
}

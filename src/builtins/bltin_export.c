/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/22 22:57:33 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Export env-var with name and value to global envlst. Return 0 if everything
 * went fine. Return -1 if f.ex. name was not correctly formatted. */
// TODO implement error checking

// FIXME: export bla="yadayada" should work as expected, i.e. set the env-var
// `zzz` to the string yadayada, without the quotes.

static char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	counter;

	counter = 0;
	while (src[counter] != '\0' && counter < n)
	{
		dest[counter] = src[counter];
		counter++;
	}
	while (counter < n)
	{
		dest[counter] = '\0';
		counter++;
	}
	return (dest);
}

char	*find_name(char *str, char *equal_pos)
{
	char	*result;

	result = malloc(equal_pos - str + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, str, equal_pos - str);
	result[equal_pos - str] = '\0';
	return (result);
}

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
	char		*name;
	char		*value;
	char		**str;

	if (cl->args[1] == NULL)
		return (0);
	str = ft_split_input(cl->args[1]);
	if (str && str[0])
	{
		name = str[0];
		if (str[1] && str[1][0])
			value = str[1];
		else
			value = NULL;
		set_env_entry(name, value, el);
	}
	free(str);
	return (0);
}

int	bltin_export_out(t_cmdlst *cl, t_envlst **el)
{
	if (cl->args[1] == NULL)
		return (print_exported_variables(*el), 0);
	return (0);
}

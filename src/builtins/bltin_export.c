/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 10:16:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_valid_identifier(const char *str);
char	*find_name(char *str, char *equal_pos);
char	**ft_split_input(char *str);
void	sort_env_list(t_envlst *env);

void	print_exported_variables(t_envlst *env)
{
	t_envlst	*env_copy;

	env_copy = envlst_copy(env);
	sort_env_list(env_copy);
	print_envlst(env_copy);
	envlst_clear(&env_copy);
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

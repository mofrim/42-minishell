/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 16:17:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* There 4 possibilities for valid export args:
 *
 * 1) export "zzz"="moep". leading tokenization: args[1]="zzz", args[2]="=" and
 * args[3]="moep"
 *
 * 2) export "zzz"=moep. leading to arg[1]="zzz", args[2]="=moep"}
 *
 * 3) export zzz="laksdjf askdjf". -> args[1]="zzz=", args[2]="laksdjf..."
 *
 * 4) export zzz=adsfs asdf -> args[1]="zzz=", args[2]="adsfs", args[3]="asdf"
 *
 * How to do `export z="z=z"` -> args[1]="z=", args[2]="z=z"
 *
 * Bash can distinguish between `export z="z=z"` -> envvar z = "z=z" and `export
 * "z=z"` -> envvar z = "z"
 *
 *
 * => somehow i need to preserve the args for export differently.
 */

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

	i = 0;
	while (cl->args[++i] != NULL)
	{
		value = NULL;
		str = ft_split_input(cl->args[i]);
		if (str && str[0])
		{
			name = str[0];
			if (!is_valid_identifier(name))
				return (1);
			if (str[1] && str[1][0])
				value = str[1];
			set_env_entry(name, value, el);
		}
		free_ptrptr(&str);
	}
	return (0);
}

int	bltin_export_out(t_cmdlst *cl, t_envlst **el)
{
	if (cl->preout_flag)
		return (1);
	if (cl->args[1] == NULL)
		print_exported_variables(*el);
	return (0);
}

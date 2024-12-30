/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:11 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/30 04:17:10 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_current_dir(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd");
		return (NULL);
	}
	return (pwd);
}

void	update_oldpwd(t_envlst *data)
{
	char	*old_pwd;

	old_pwd = get_current_dir();
	if (old_pwd)
	{
		update_env_value(data, "OLDPWD", old_pwd);
		free(old_pwd);
	}
}

void	update_pwd(t_envlst *data)
{
	char	*new_pwd;

	new_pwd = get_current_dir();
	if (new_pwd)
	{
		update_env_value(data, "PWD", new_pwd);
		free(new_pwd);
	}
}

int	cd_home(t_envlst **env)
{
	t_envlst	*ptr;

	ptr = *env;
	while (ptr && !(ft_strcmp(ptr->name, "HOME") == 0))
		ptr = ptr->next;
	if (!ptr || chdir(ptr->value) != 0)
	{
		if (!ptr)
			ft_printf("minishell: cd: HOME not set\n");
		else
			ft_printf("minishell: cd: %s: %s\n", ptr->value, strerror(errno));
		return (1);
	}
	return (0);
}

int	bltin_cd(char **arg, t_envlst **env)
{
	int	i;

	i = 0;
	if (array_length(arg) > 2)
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	update_oldpwd(*env);
	if (array_length(arg) == 1)
		i = cd_home(env);
	else if (chdir(arg[1]) != 0)
	{
		ft_printf("minishell: cd: %s: %s\n", arg[1], strerror(errno));
		i = 1;
	}
	if (i == 0)
		update_pwd(*env);
	return (i);
}

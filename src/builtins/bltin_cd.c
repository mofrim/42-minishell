/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:11 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/20 01:58:29 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Set the PWD and OLDPWD env vars to their new values. */
int	update_xpwd(t_envlst **el, char *oldpwd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		set_env_entry("PWD", pwd, el);
		free(pwd);
	}
	else
		return (errno);
	set_env_entry("OLDPWD", oldpwd, el);
	return (0);
}

/* Return values:
 * 	42: Home is not set
 * 	else: errno */
int	cd_home(t_envlst **el)
{
	t_envlst	*ptr;

	ptr = get_env_entry_by_name("HOME", *el);
	if (!ptr || chdir(ptr->value) != 0)
	{
		if (!ptr)
			return (42);
		else
			return (errno);
	}
	return (0);
}

int	cd_dash(t_envlst **el, int len)
{
	t_envlst	*ptr;
	char		*current_pwd;

	ptr = get_env_entry_by_name("OLDPWD", *el);
	current_pwd = getcwd(NULL, 0);
	if (!ptr || (ft_strcmp(ptr->value, current_pwd) == 0))
		return (minish_errormsg("cd:", "OLDPWD not set", errno));
	else if (chdir(ptr->value) != 0)
	{
		free(current_pwd);
		return (minish_errormsg("cd", ptr->value, errno));
	}
	if (len == 1)
		ft_printf("%s\n", ptr->value);
	free(current_pwd);
	return (0);
}

int	bltin_cd_preout(t_cmdlst *cl, t_envlst **el)
{
	int		retval;
	char	*oldpwd;

	retval = 1;
	if (cl->arg_count <= 2)
	{
		oldpwd = getcwd(NULL, 0);
		if (!oldpwd)
			return (errno);
		if (cl->arg_count == 1)
			retval = cd_home(el);
		else if (ft_strcmp(cl->args[1], "-") == 0
			|| ft_strcmp(cl->args[1], "--") == 0)
			retval = cd_dash(el, ft_strlen(cl->args[1]));
		else
			retval = chdir(cl->args[1]);
		if (retval == 0)
		{
			retval = update_xpwd(el, oldpwd);
			free(oldpwd);
		}
	}
	if (retval == -1)
		return (free(oldpwd), errno);
	return (retval);
}

int	bltin_cd_out(t_cmdlst *cl, t_envlst **el)
{
	(void)el;
	if (cl->arg_count > 2)
		return (minish_errormsg("cd", "too many arguments", 1));
	if (cl->preout_flag == 42)
		return (minish_errormsg("cd", "HOME not set", 1));
	if (cl->preout_flag)
		return (ft_dprintf(2, "minishell: cd: %s: %s\n", cl->args[1],
				strerror(cl->preout_flag)), 1);
	else
		return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:09:09 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:09:11 by fmaurer          ###   ########.fr       */
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

/* Handle `cd -`. */
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

/* The cd should have an effect in main process (that is: changing the curent
 * dir ;). So we need to do this in preout, i.e. not inside child process. */
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

/* The output realted things happening in child process. */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 23:45:32 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 11:20:46 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Set the ? envvar.
 *
 * Only set it if we are in normal interactive mode or, when minishell gets
 * prompt via pipe, the old value of $? is not 2 indicating a syntax error from
 * before.
 */
void	set_exit_status_envvar(char *status_str, t_envlst **el)
{
	if (isatty(STDIN_FILENO))
		set_env_entry("?", status_str, el);
	else if (ft_atoi(get_env_value("?", *el)) != 2)
		set_env_entry("?", status_str, el);
	free(status_str);
}

void	init_shell_vars(t_envlst **el)
{
	char	*shlvl_cnt;
	char	*pwd;

	set_env_entry("?", "0", el);
	if (get_env_entry_by_name("SHLVL", *el))
	{
		shlvl_cnt = ft_itoa(ft_atoi(get_env_value("SHLVL", *el)) + 1);
		set_env_entry("SHLVL", shlvl_cnt, el);
		free(shlvl_cnt);
	}
	else
		set_env_entry("SHLVL", "1", el);
	if (!get_env_value("PATH", *el))
		set_env_entry("PATH", "/no-such-path", el);
	set_env_entry("OLDPWD", NULL, el);
	pwd = ft_calloc(1024, sizeof(char));
	getcwd(pwd, 500);
	set_env_entry("PWD", pwd, el);
	free(pwd);
}

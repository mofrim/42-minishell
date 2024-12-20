/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:46 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/20 13:36:42 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_vars(t_envlst *el)
{
	while (el)
	{
		if (el->value)
			ft_printf("%s=\"%s\"\n", el->name, el->value);
		el = el->next;
	}
	return ;
}

static int	bltin_env_out(t_envlst *env, char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i > 1)
	{
		ft_printf("minishell: env: %s: No such file or directory\n", str[1]);
		return (1);
	}
	print_env_vars(env);
	return (0);
}

int	bltin_env(t_cmdlst *cmdl, t_envlst *el)
{
	int	exit_status;
	int	cpid;

	exit_status = 0;
	cpid = fork();
	if (cpid == -1)
		return (errno);
	if (cpid == 0)
	{
		if (open_redir_files(cmdl->input_file, cmdl->outfiles))
			exit(errno);
		exit(bltin_env_out(el, cmdl->args));
	}
	waitpid(cpid, &exit_status, 0);
	return (exit_status >> 8);
}

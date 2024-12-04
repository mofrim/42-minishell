/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_exec_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:48:38 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/04 15:10:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* For struct stat, stat() and S_IFREG. */
#include <sys/stat.h>

char		**get_path_from_env(char **env);
static char	*join_exec_path_strings(char *path, char *exec);
static int	ft_initial_strcmp(char *s1, char *s2);

/* Get the real executable path of cmd from PATH envvar. First checks if file is
 * directly executable (and not a DIR with X-bit set!). If it isn't tries to
 * construct exec_path using PATH. */
// FIXME: we will have to the PATH from *our* env-list here! Because it should
// be possible to do an export PATH=$PATH:./somedir/ before executing something
// by name. So in the end we need this function to determine the real exec_path
// and call execve like `execve(cmd, args, NULL)}`
char	*get_exec_path(t_cmdlst *clst, char **env)
{
	struct stat	sb;
	int			i;
	char		**path_split;
	char		*exec_path;

	if (stat(clst->cmd, &sb) == 0)
		if ((sb.st_mode & S_IFREG) == S_IFREG && !access(clst->cmd, X_OK))
			return (clst->cmd);
	path_split = get_path_from_env(env);
	i = -1;
	while (path_split[++i])
	{
		exec_path = join_exec_path_strings(path_split[i], clst->cmd);
		if (access(exec_path, X_OK) == 0)
		{
#ifdef DEBUG
			ft_printf(RED "<< DEBUG >> i really can access X_OK: %s\n" RST, \
					exec_path);
#endif
			free_ptrptr(&path_split);
			return (exec_path);
		}
		free(exec_path);
	}
	free_ptrptr(&path_split);
	return (NULL);
}

char	**get_path_from_env(char **env)
{
	int		i;
	char	**path_split;

	if (!env || !env[0])
		error_exit("empty env");
	i = 0;
	while (!ft_initial_strcmp(env[i], "PATH") && env[i])
		i++;
	if (!env[i])
		error_exit("no PATH found in env");
	path_split = ft_split((env[i] + 5), ':');
	if (!path_split)
		error_exit("malloc in get_path_from_env ft_split failed");
	return (path_split);
}

char	*join_exec_path_strings(char *path, char *exec)
{
	char	*path_slash;
	char	*exec_path;

	path_slash = ft_strjoin(path, "/");
	nullcheck(path_slash, "path_slash malloc failed");
	exec_path = ft_strjoin(path_slash, exec);
	nullcheck(exec_path, "exec_path malloc failed");
	free(path_slash);
	return (exec_path);
}

int	ft_initial_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (0);
		s1++;
		s2++;
	}
	return (1);
}

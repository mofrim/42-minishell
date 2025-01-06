/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_exec_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:48:38 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 17:10:59 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* For struct stat, stat() and S_IFREG. */
#include <sys/stat.h>

static char	**get_path_from_env(char **env);
static char	*join_exec_path_strings(char *path, char *exec);
static int	ft_initial_strcmp(char *s1, char *s2);
static int	check_exec_path(char *exec_path);

/* Get the real executable path of cmd from PATH envvar. First checks if file is
 * directly executable (and not a DIR with X-bit set!). If it isn't tries to
 * construct exec_path using PATH. 
 * 1) return NULL if command is really not found
 * 2) return "" if file is found but not executable 
 *
 * Return values of used functions:
 * - access: return != 0 if file-mode not correct.
 * - stat: return 0 if everything is fine, some file was found
 *	
 * */
char	*get_exec_path(t_cmdlst *cl, char **env)
{
	int		i;
	char	**path_split;
	char	*exec_path;

	if (check_exec_path(cl->cmd) == 1)
		return (ft_strdup(cl->cmd));
	if (check_exec_path(cl->cmd) == 2)
		return (ft_strdup(""));
	if (check_exec_path(cl->cmd) == 3)
		return (NULL);
	path_split = get_path_from_env(env);
	i = -1;
	while (path_split[++i])
	{
		exec_path = join_exec_path_strings(path_split[i], cl->cmd);
		if (check_exec_path(exec_path) == 1)
			return (free_ptr2ptr(&path_split), exec_path);
		if (check_exec_path(exec_path) == 2)
			return (ft_strdup(""));
		free(exec_path);
	}
	free_ptr2ptr(&path_split);
	return (NULL);
}

/* Check possible exec_path for validity.
 * Return Values:
 * 0: exec_path is not accessible
 * 1: exec_path is a directly executable file
 * 2: exec_path is a file but not executable
 * 3: exec_path is a dir and executable
 *
 * For security reasons: ignore all paths not starting with '/' or "./". This is
 * also what bash does.
 * */
static int	check_exec_path(char *exec_path)
{
	struct stat	sb;

	if (exec_path && !(exec_path[0] == '/' || \
		(ft_strlen(exec_path) > 2 && !ft_strncmp(exec_path, "./", 2))))
		return (0);
	if (!stat(exec_path, &sb))
	{
		if (((sb.st_mode & S_IFREG) == S_IFREG) && !access(exec_path, X_OK))
			return (1);
		if (((sb.st_mode & S_IFREG) == S_IFREG) && access(exec_path, X_OK))
			return (2);
		if (((sb.st_mode & S_IFDIR) == S_IFDIR) && !access(exec_path, X_OK))
			return (3);
	}
	return (0);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_get_exec_path.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:48:38 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/22 13:17:15 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_path_from_env(char **env);
static char	*join_exec_path_strings(char *path, char *exec);
static int	check_exec_path(char *exec_path, char **path_env);
static int	ft_initial_strcmp(char *s1, char *s2);

/**
 * Get the real executable path of cmd from PATH envvar.
 *
 * First checks if file is directly executable (and not a DIR with X-bit set!).
 * If it isn't tries to construct exec_path using PATH.
 *
 * 0) return cmd-string as-is if directly executable, f.ex. `/usr/bin/env`
 * 1) return NULL if command is really not found or there is something wrong
 *    with the env
 * 2) return "" if file is found but not executable
 */
char	*get_exec_path(t_cmdlst *cl, char **env)
{
	int		i;
	char	**envpath;
	char	*exec_path;

	envpath = get_path_from_env(env);
	if (check_exec_path(cl->cmd, envpath) == 1)
		return (ft_strdup(cl->cmd));
	if (check_exec_path(cl->cmd, envpath) == 2)
		return (ft_strdup(""));
	if (check_exec_path(cl->cmd, envpath) == 3)
		return (NULL);
	i = -1;
	while (envpath && envpath[++i])
	{
		exec_path = join_exec_path_strings(envpath[i], cl->cmd);
		if (check_exec_path(exec_path, envpath) == 1)
			return (free_ptr2ptr(&envpath), exec_path);
		if (check_exec_path(exec_path, envpath) == 2)
			return (ft_strdup(""));
		free(exec_path);
	}
	free_ptr2ptr(&envpath);
	return (NULL);
}

/**
 * Check possible exec_path for validity.
 *
 * Return Values:
 * 0: exec_path does not start with "/" or "./" -> try PATH from env
 * 1: exec_path is a directly executable file
 * 2: exec_path is a file but not executable
 * 3: exec_path is a dir and executable or cmd starts with "/" or "./" but is
 *    not found or PATH is not set and exec_path is not directly executable.
 *
 * For security reasons: ignore all paths not starting with '/' or "./". This is
 * also what bash does.
 *
 * Return values of used functions:
 * - access: return != 0 if file-mode not correct.
 * - stat: return 0 if everything is fine, some file was found
 * */
static int	check_exec_path(char *exec_path, char **path_env)
{
	struct stat	sb;

	if (exec_path && path_env && (!(exec_path[0] == '/' || \
		(ft_strlen(exec_path) > 2 && !ft_strncmp(exec_path, "./", 2))) && \
				!ft_strchr(exec_path, '/')))
		return (0);
	if (exec_path && (stat(exec_path, &sb) == -1) && ((exec_path[0] == '/' || \
		(ft_strlen(exec_path) > 2 && !ft_strncmp(exec_path, "./", 2))) || \
			path_env == NULL))
		return (3);
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

/**
 * Get the $PATH from env.
 *
 * If $PATH is not set in env or if something is wrong with env (no env at
 * all?!) return NULL. If we find $PATH return a split of it.
 */
char	**get_path_from_env(char **env)
{
	int		i;
	char	**path_split;

	if (!env || !*env)
		return (NULL);
	i = 0;
	while (ft_initial_strcmp(env[i], "PATH") && env[i])
		i++;
	if (!env[i])
		return (NULL);
	if (env[i][0] == '\0')
		return (NULL);
	path_split = ft_split((env[i] + 5), ':');
	nullcheck(path_split, "get_path_from_env");
	return (path_split);
}

/* Join $PATH path-strings with our cmd-string. */
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

/* Simple initial-string-cmp. Somehow ft_strncmp did cause to much trouble here
 * or was to unsafe regarding possible NULLs. */
int	ft_initial_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (1);
		s1++;
		s2++;
	}
	return (0);
}

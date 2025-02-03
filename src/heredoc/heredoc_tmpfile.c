/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_tmpfile.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:56:54 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 09:34:09 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Some local functions implemented below. */
static t_htmpfile	*try_mktmpfile(char *filename);
static void			set_random_tmpfilename(char *tmpfile);
static int			rand_uint(void);

/**
 * The temp file creation function.
 *
 * To be called like `mktmpfile(heredoc_DLIM_XXXXXX)`. Then tries to create a
 * temporary file with the last 6 chars randomized and returns its fd number. If
 * doesn't succeed, returns NULL. 
 */
static t_htmpfile	*mktmpfile(char *template)
{
	size_t		len;
	int			tries;
	char		*filename;
	t_htmpfile	*htf;

	filename = ft_strdup(template);
	nullcheck(filename, "mktmpfile");
	len = ft_strlen(filename);
	if (len < 6 || ft_strcmp(filename + len - 6, "XXXXXX") != 0)
		return (NULL);
	tries = -1;
	while (++tries < 100)
	{
		set_random_tmpfilename(filename);
		htf = try_mktmpfile(filename);
		if (htf != NULL)
			return (free(filename), htf);
	}
	return (free(filename), NULL);
}

static void	set_random_tmpfilename(char *tmpfile)
{
	char	*chars;
	size_t	len;
	size_t	i;

	len = ft_strlen(tmpfile);
	chars = ft_strdup(
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
			);
	nullcheck(chars, "set_random_tmpfilename()");
	i = len - 6 - 1;
	while (++i < len)
		tmpfile[i] = chars[rand_uint() % 62];
	free(chars);
}

/**
 * Returns a random number between 0 and RAND_MAX == INT_MAX.
 *
 * Returns -1 on error.
 * The RAND_MAX + 1UL ensures that the calculation is done in unsigned int even
 * if RAND_MAX is signed int.
 */
static int	rand_uint(void)
{
	int				fd;
	unsigned int	random_value;
	ssize_t			bytes_read;

	fd = open("/dev/random", O_RDONLY);
	if (fd == -1)
		return (-1);
	bytes_read = read(fd, &random_value, sizeof(random_value));
	close(fd);
	if (bytes_read != sizeof(random_value))
		return (-1);
	return ((int)(random_value % (RAND_MAX + 1UL)));
}

/* Try to create a tmpfile. */
static t_htmpfile	*try_mktmpfile(char *filename)
{
	int			fd;
	t_htmpfile	*htf;

	htf = malloc(sizeof(t_htmpfile));
	nullcheck(htf, "try_mktmpfile()");
	fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd >= 0)
	{
		htf->fd = fd;
		htf->filename = ft_strdup(filename);
		return (htf);
	}
	else
	{
		free(htf);
		return (NULL);
	}
}

/* Wrapper around mktmpfile for creating tmpfile with the pattern
 * "/tmp/minish_heredoc-XXXXXX". */
t_htmpfile	*create_heredoc_tmpfile(void)
{
	return (mktmpfile("/tmp/minish_heredoc-XXXXXX"));
}

// Version with setting errno... good style paragon!
//
// int	mktmpfile(char *template)
// {
// 	char *chars;
// 	size_t	len;
// 	int		tries;
//
// 	len = ft_strlen(template);
// 	chars = ft_strdup("
// 	abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
// 	if (len < 6 || ft_strcmp(template + len - 6, "XXXXXX") != 0)
// 	{
// 		errno = EINVAL;
// 		return (-1);
// 	}
// 	tries = 0;
// 	while (tries < 100) 
// 	{
// 		for (size_t i = len - 6; i < len; i++) 
// 			template[i] = chars[rand() % (sizeof(chars) - 1)];
// 		int fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
// 		if (fd >= 0)
// 			return (fd);
// 		if (errno != EEXIST)
// 			return (-1);
// 		tries++;
// 	}
// 	errno = EEXIST;
// 	free(chars);
// 	return (-1);
// }

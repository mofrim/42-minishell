/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:12:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/14 10:35:36 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Your question was cut off, but I assume you're asking about how HEREDOC is
// implemented in shells. Let me explain the key mechanisms:
//
//     1. Parsing phase :
//     - Shell identifies the HEREDOC delimiter(e.g., 'EOL')
//     - Everything between the opening `<<` and the terminating delimiter is
//     stored as a string
//     - Newlines are preserved exactly as written
//
//     2. Variable expansion rules :
//
//		   ```bash
//		   # Without quotes - variables expand
//			   cat << EOL
//			   $USER
//			   EOL
//
//		   # With quotes - variables treated literally
//				   cat << 'EOL'
//				   $USER
//				   EOL
//		   ```
//
//     3. Implementation details :
//     - Shell creates a temporary file or memory buffer
//     - Content between delimiters is written to this buffer
//     - When execution reaches the HEREDOC, this buffer is fed to
//     the command's stdin
//     - Buffer is cleaned up after command completes
//
//     4. Key implementation checks :
//     - Delimiter must be first thing on its terminating line
//     - No extra spaces / tabs allowed by default(unless tab suppression is
//      used)
//     - Nested HEREDOCs are tracked with a stack
//

extern int	g_signal;

static int	do_the_heredoc(t_cmdlst *cl, t_envlst *el);
t_htmpfile	*create_heredoc_tmpfile(void);
void		substitute_envvars(char **prompt_input, t_envlst *el);

int	heredoc(t_cmdlst *cl, t_envlst *el)
{
	int	status;

	status = 0;
	signal(SIGINT, heredoc_sig_handler);
	while (cl)
	{
		if (cl->heredocs)
			status = do_the_heredoc(cl, el);
		cl = cl->next;
	}
	signal(SIGINT, sigint_handler);
	return (status);
}

static void	set_redirlst_infile_to_tmpfile(t_cmdlst *cl,
		t_htmpfile *tmpfile, char *dlim)
{
	t_redirlst	*rl;

	rl = cl->redirs;
	while (rl)
	{
		if (rl->redtype == RE_HERE && !ft_strcmp(rl->infile, dlim))
		{
			free(rl->infile);
			rl->infile = ft_strdup(tmpfile->filename);
			rl->fd_from = tmpfile->fd;
		}
		rl = rl->next;
	}
}

static void	cleanup_ptrs_and_tmpfile(t_htmpfile *tmpfile, char	*input,
		t_herdlst **hl)
{
	if (input == NULL)
	{
		close(tmpfile->fd);
		free(tmpfile->filename);
		free(tmpfile);
	}
	else
	{
		close(tmpfile->fd);
		free(input);
		free(tmpfile->filename);
		free(tmpfile);
		*hl = (*hl)->next;
	}
}

static	void	process_heredoc_prompt(char **input, t_htmpfile *tmpfile,
		t_envlst *el)
{
	substitute_envvars(input, el);
	ft_dprintf(tmpfile->fd, "%s\n", *input);
	free(*input);
	*input = readline("> ");
}

static int	do_the_heredoc(t_cmdlst	*cl, t_envlst *el)
{
	char		*input;
	t_herdlst	*hl;
	t_htmpfile	*tmpfile;

	hl = cl->heredocs;
	while (hl)
	{
		tmpfile = create_heredoc_tmpfile();
		if (tmpfile == NULL)
			return (minish_errormsg("heredoc", \
						"heredoc tmpfile creation failed", -1));
		set_redirlst_infile_to_tmpfile(cl, tmpfile, hl->name);
		input = readline("> ");
		while (input != NULL && ft_strcmp(hl->name, input) && !g_signal)
			process_heredoc_prompt(&input, tmpfile, el);
		if (input == NULL)
			return (cleanup_ptrs_and_tmpfile(tmpfile, input, &hl), 1);
		else
			cleanup_ptrs_and_tmpfile(tmpfile, input, &hl);
	}
	return (0);
}

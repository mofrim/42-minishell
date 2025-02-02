/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:12:39 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/02 16:04:06 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * The one only global var.
 *
 * Why do we need it? When Ctrl-C is pressed and SIGINT is send to our shell,
 * the heredoc prompt should stop, "^C" should be printed and the normal shell
 * prompt should re-appear. So we need to transfer some information to the
 * heredoc reading function to stop processing the heredocs and cleanup. This
 * could not be done without establishing a connection between the
 * signal-handling function and the heredoc-func.
 * For our normal minishell signal handling we do not need this because, we can
 * handle all this via the readline variables and functions dirtily manipulating
 * the global readline state.
 */
extern int	g_signal;

/* Functions only used in this file. */
static int	do_the_heredoc(t_cmdlst	*cl, t_ministruct *mini);
t_htmpfile	*create_heredoc_tmpfile(void);
void		substitute_envvars(char **prompt_input, t_envlst *el);

/**
 * Main herdoc wrapper function.
 *
 * Scans through the whole cmdlst and processes heredocs for each heredoc we
 * find. By this automatically the linear behavior of bash is being reproduced.
 * For the heredoc phase the signal handling is changed to mimic bash's
 * behavior.
 */
int	heredoc(t_cmdlst *cl, t_ministruct *mini)
{
	int	status;

	status = 0;
	signal(SIGINT, heredoc_sig_handler);
	while (cl)
	{
		if (cl->heredocs)
			status = do_the_heredoc(cl, mini);
		if (status == 2)
			break ;
		cl = cl->next;
	}
	signal(SIGINT, sigint_handler);
	return (status);
}

/* Replace the infile in the redirlst of the current cmd (which should be set to
 * the name of the current heredoc delim) by the tmpfile we use for buffering
 * heredoc input. */
static void	set_redirlst_infile_to_tmpfile(t_cmdlst *cl,
		t_htmpfile *tmpfile, char *dlim)
{
	t_redirlst	*rl;

	rl = cl->redirs;
	while (rl)
	{
		if ((rl->redtype == RE_DOC || rl->redtype == RE_QDOC) && \
				!ft_strcmp(rl->infile, dlim))
		{
			free(rl->infile);
			rl->infile = ft_strdup(tmpfile->filename);
			rl->fd_from = tmpfile->fd;
		}
		rl = rl->next;
	}
}

/* Handle the end of one heredoc read. Also responsible for generating the
 * correct return value for the do_the_heredoc func depending on the termination
 * of this heredoc session. See comment there for retval explanation. */
static int	cleanup_and_reset_sig(t_htmpfile *tmpfile, char	*input,
		t_herdlst **hl)
{
	int	retval;

	retval = 0;
	close(tmpfile->fd);
	free(tmpfile->filename);
	free(tmpfile);
	if (input == NULL)
	{
		ft_dprintf(STDERR_FILENO,
			"minish: heredoc: terminated by EOF, not by %s\n", (*hl)->name);
		retval = 1;
	}
	else
	{
		free(input);
		*hl = (*hl)->next;
	}
	if (g_signal)
	{
		g_signal = 0;
		retval = 2;
	}
	return (retval);
}

/* Process the heredoc prompt and redisplay. If the heredoc delim wasn't quoted
 * substitute possible envvars in the heredoc input. */
static void	process_heredoc_prompt(char **input, t_htmpfile *tmpfile,
		t_ministruct *mini, t_toktype hdoctype)
{
	if (hdoctype == TOK_HERE_DLIM)
		substitute_envvars(input, mini->el);
	ft_dprintf(tmpfile->fd, "%s\n", *input);
	free(*input);
	read_prompt(input, "> ");
}

/**
 * Heart of the heredoc.
 *
 * Process the heredoc list for one command (yes, one command might have several
 * heredocs). First for each heredoc-delimiter there is tmpfile created, then
 * this tmpfile is added to the cmdlist entry we are currently processing the
 * heredoc of as infile (stdin redirect). The heredoc prompt is then ft_dprinted
 * line by line to the tmpfile, iff we did not receive a signal (ctrl-c was
 * pressed) or the prompt was ended by entering the delim or pressing ctrl-d.
 *
 * Return Values:
 * 	0)	Normal operation. Prompt finished by delim.
 * 	1)	Termination by Ctrl-D.
 * 	2)	Termination by Ctrl-C -> quit cmdlst processing.
 */
static int	do_the_heredoc(t_cmdlst	*cl, t_ministruct *mini)
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
		read_prompt(&input, "> ");
		while (input != NULL && ft_strcmp(hl->name, input) && !g_signal)
			process_heredoc_prompt(&input, tmpfile, mini, hl->type);
		if (input == NULL)
			return (cleanup_and_reset_sig(tmpfile, input, &hl));
		else if (g_signal)
			return (cleanup_and_reset_sig(tmpfile, input, &hl));
		else
			cleanup_and_reset_sig(tmpfile, input, &hl);
	}
	return (0);
}

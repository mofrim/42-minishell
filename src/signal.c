/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:24:37 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/02 21:19:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The global var. For exhaustive comment & explanation see
 * src/heredoc/heredoc.c. Only needed in heredoc sig-handling. */
int	g_signal;

/**
 * Main minishell signal handler.
 *
 * SIGTERM handling is just for fun and useless. But overall handling with
 * readline was quite hard to get right in the first place. Explanation:
 *
 *	- `rl_replace_line("", 1)` Sets the readline buffer to "" and clears the
 *		undo history (2nd param, the "1" indicates this
 *	- `rl_on_new_line()` Tell readline's internal state that we are on a new
 *		line, which is the case after either printing "^C\n" or the other msg.
 *	- `rl_redisplay()` Redraw the current from readline's internal state. Clear
 *		text: Output the prompt as if it was the first time.
 */
void	minish_sighandler(int signum)
{
	if (signum == SIGINT)
		ft_printf("^C\n");
	else
		ft_dprintf(STDERR_FILENO, "\nGot signaled! But not SIGKILL. Puh!\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * The heredoc signal handler.
 *
 * Sets the global var to 1 in order to make heredoc-function exit cleanly. Also
 * a bit of a hack using ioctl() was necessary to make the heredoc behave like
 * bash on ctrl-c. `ioctl(STDIN_FILENO, TIOCSTI, "\n")` sends a '\n' to the
 * terminals stdin, similuating a pressed enter key. This way and by seting
 * rl_done = 1 readline cleanly finishes the line reading.
 */
void	heredoc_sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signal = 1;
		ft_printf("^C");
		if (ioctl(STDIN_FILENO, TIOCSTI, "\n") == -1)
			perror("ioctl failed");
		rl_done = 1;
	}
}

/**
 * Minishell signal setup.
 *
 * Maybe a bit overkill, bc we do not set any sa_flags. So, we could have also
 * done it using the signal() function.
 */
int	signal_setup(void (*sig_handler)(int))
{
	struct sigaction	sig_act;

	rl_catch_signals = 0;
	memset(&sig_act, 0, sizeof(struct sigaction));
	sig_act.sa_handler = sig_handler;
	sig_act.sa_flags = 0;
	sigemptyset(&sig_act.sa_mask);
	if (sigaction(SIGINT, &sig_act, NULL) == -1)
		return (minish_errormsg("signal_setup",
				"setting sigaction failed", -1));
	if (sigaction(SIGTERM, &sig_act, NULL) == -1)
		return (minish_errormsg("signal_setup",
				"setting sigaction failed", -1));
	return (0);
}

/* Check if exit status of process indicates termination by signal.
 * This is the code of WIFSIGNALED macro directly copied from stdlib.h 8-P.
 * 0x7f == 0b1111111 == 127, thats where the signal is stored in exit_status.*/
int	ft_wifsignaled(int status)
{
	return ((((status & 0x7f) + 1) >> 1) > 0);
}

/* Return exit status from a satus retrieved by wait/waitpid.
 * Again, code of WEXTISTATUS copied from stdlib.h. 
 * 0xff00 == 0b1111111100000000, thats where the exit_status is stored. */
int	ft_wexitstatus(int status)
{
	return ((status & 0xff00) >> 8);
}

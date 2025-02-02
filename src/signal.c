/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:24:37 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/01 14:19:15 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

/**
 * Main signal handler.
 *
 * Description.
 */
void	sigint_handler(int signum)
{
	(void)signum;
	while (wait(NULL) != -1 && errno != ECHILD)
		;
	ft_printf("^C\n");
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

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

/* QUESTION: do i need this? doesn't a simple call to signal() with individual
 * functions for each function suffice? 
 */
void	signal_setup(void (*sig_handler)(int))
{
	struct sigaction	sig_act;

	rl_catch_signals = 0;
	sig_act.sa_handler = sig_handler;
	sig_act.sa_flags = 0;
	sigemptyset(&sig_act.sa_mask);
	sigaction(SIGINT, &sig_act, NULL);
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

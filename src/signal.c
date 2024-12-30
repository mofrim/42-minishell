/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:24:37 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/30 10:39:00 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME... this is not correct so far. when i run minishell inside of minishell
// prompt is displayed twice or even 4 times.
//
// NOTE Oh damn! i have got kill()! so i can just record all pid's for all child
// processes and kill them if ctrl-c is hit! if ctrl-c is hit, the the g_sig
// global variable will be set and in the wait()-loop in exec...() g_sig might
// be tested. if set: kill all children.
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

//// Some ideas for signal handling from slack..
//
// rl_done = 1;
// if (ioctl(STDIN_FILENO, TIOCSTI, "\n") == -1)
//        perror("ioctl failed");
// close(STDIN_FILENO);

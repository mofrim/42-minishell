/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 23:24:37 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/18 18:34:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "minishell.h"
#include "readline/readline.h"

/* Works so far. */
void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

/* QUESTION: do i need this? doesn't a simple call to signal() with individual
 * functions for each function suffice? */
void	signal_setup(void (*sig_handler)(int))
{
	struct sigaction	sig_act;

	sig_act.sa_handler = sig_handler;
	sig_act.sa_flags = 0;
	sigemptyset(&sig_act.sa_mask);
	sigaction(SIGINT, &sig_act, NULL);
}

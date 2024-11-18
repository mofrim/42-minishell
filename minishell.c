/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/18 18:37:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <termios.h>

int	main(int ac, char **av, char **envp)
{
	t_termios old_settings;
	char	*input;
	
	// NOTE: don't need this but will keep for later info.
	// rl_catch_signals = 0;
	signal_setup(signal_handler);
	term_setup(&old_settings);

	// FIXME this is still kind of ugly... what should i do?
	input = malloc(1);

	while(input)
	{
		free(input);
		input = readline("$ ");
	}

	// FIXME handle Ctrl-D like bash
	ft_printf("exit\n");

	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return (0);
}

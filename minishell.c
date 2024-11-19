/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/20 00:09:32 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>

int	main(int ac, char **av, char **envp)
{
	t_termios old_settings;
	char	*input;
	
	// NOTE: don't need this but will keep for later info.
	// rl_catch_signals = 0;
	signal_setup(signal_handler);
	term_setup(&old_settings);

	while(1)
	{
		input = readline("$ ");
		if (!input)
		{
			printf("exit");
			tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
			exit(0);
		}
		add_history(input);
		t_tokenlist *tlst = tokenize(input);
		t_tokenlist *tmp = tlst;

		while (tlst) {
			printf("token: %d, value: %s\n", tlst->token->type, tlst->token->value);
			tlst = tlst->next;
		}

		tlst = tmp;
		t_cmdlst *cmdlst = parse_tokenlist(tlst);
		print_cmdlst(cmdlst);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return (0);
}

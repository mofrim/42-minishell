/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/28 17:15:46 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <termios.h>

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	int			bla;

	// NOTE: don't need this but will keep for later info.
	// rl_catch_signals = 0;
	signal_setup(signal_handler);
	term_setup(&old_settings);
	while (1)
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

		if (tlst)
		{
			while (tlst)
			{
				ft_printf("token: ");
				print_tokentype(tlst->token);
				ft_printf(", value: %s\n", tlst->token->value);
				tlst = tlst->next;
			}
		}

		// tlst = tmp;
		// t_cmdlst *cmdlst = parse_tokenlist(tlst);
		// print_cmdlst(cmdlst);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return (0);
}

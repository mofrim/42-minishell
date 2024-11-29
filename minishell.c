/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/29 17:16:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "readline/readline.h"
#include <stdio.h>
#include <termios.h>

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	int			bla;
	t_tokenlist *tlst;

#ifdef DEBUG
	ft_printf("<< DEBUG >> debug mode\n");
	// print_env(envp);
#endif

	// NOTE: don't need this but will keep for later info.
	// rl_catch_signals = 0;
	signal_setup(signal_handler);
	term_setup(&old_settings);
	tlst = NULL;
	while (1)
	{
		input = readline("$ ");
		if (!input)
		{
			printf("exit");
			tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
			rl_clear_history();
			if (tlst)
				toklst_clear(&tlst);
			exit(0);
		}
		add_history(input);
		tlst = tokenize(input);
		//
		// if (tlst)
		// 	print_toklst(tlst);

		// tlst = tmp;
		// t_cmdlst *cmdlst = parse_tokenlist(tlst);
		// print_cmdlst(cmdlst);
		toklst_clear(&tlst);
		free(input);
	}
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return (0);
}

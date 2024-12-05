/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 12:03:37 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	t_toklst *tlst;
	t_envlst	*el;
	t_cmdlst	*cmdlst;

	el = parse_env(envp);
#ifdef DEBUG
	ft_printf("<< DEBUG >> debug mode\n");
	// print_envlst(el);
	// print_env(envp);
#endif

	// NOTE: don't need this but will keep for later info.
	// rl_catch_signals = 0;
	signal_setup(signal_handler);
	term_setup(&old_settings);
	tlst = NULL;
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			printf("exit");
			tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
			rl_clear_history();
			envlst_clear(&el);
			if (tlst)
				toklst_clear(&tlst);
			exit(0);
		}
		add_history(input);
		tlst = tokenize(input);
		if (tlst)
		{
			(void)write(1, "\n", 1);
			cmdlst = parse_tokenlist(tlst);
#ifdef DEBUG
			ft_printf(RED "<< DEBUG >> toklist after lvl2:\n" RST);
			print_toklst(tlst);
			ft_printf(RED "<< DEBUG >> cmdlst:\n" RST);
			print_cmdlst(cmdlst);
#endif
			int status = exec_cmd(cmdlst, el);
			ft_printf(RED "<< DEBUG >> exit status exec_cmd() = %d\n" RST, status);
			(void)write(1, "\n", 1);
			cmdlst_clear(&cmdlst);
			toklst_clear(&tlst);
		}
		free(input);
	}
	rl_clear_history();
	envlst_clear(&el);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return (0);
}

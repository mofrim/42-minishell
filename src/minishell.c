/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 11:29:45 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_and_exit(t_termios *old_settings, t_envlst **el, \
		t_toklst **tl);
static void	evaluate_cmdline(t_toklst **tl, t_envlst *el);

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	t_toklst	*tlst;
	t_envlst	*el;

	el = parse_env(envp);
	signal_setup(signal_handler);
	term_setup(&old_settings);
	tlst = NULL;
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			cleanup_and_exit(&old_settings, &el, &tlst);
		add_history(input);
		tlst = tokenize(input, el);
		if (tlst)
			evaluate_cmdline(&tlst, el);
		free(input);
	}
	return (0);
}

static void	evaluate_cmdline(t_toklst **tl, t_envlst *el)
{
	t_cmdlst	*cl;
	int			status;

	cl = parse_tokenlist(*tl);
	status = exec_cmd(cl, el);
	cmdlst_clear(&cl);
	toklst_clear(tl);
}

static void	cleanup_and_exit(t_termios *old_settings, t_envlst **el, \
		t_toklst **tl)
{
	ft_printf("exit");
	tcsetattr(STDIN_FILENO, TCSANOW, old_settings);
	rl_clear_history();
	envlst_clear(el);
	if (*tl)
		toklst_clear(tl);
	exit(0);
}

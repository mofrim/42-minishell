/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/12 15:26:38 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_and_exit(t_termios *old_settings, t_envlst **el, \
		t_toklst **tl);
static void	evaluate_cmdline(t_toklst **tl, t_envlst **el);
static void	init_shell(t_envlst **el, t_termios	*old_settings, t_toklst **tl, \
		char **envp);

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	t_toklst	*tlst;
	t_envlst	*el;

	init_shell(&el, &old_settings, &tlst, envp);
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			cleanup_and_exit(&old_settings, &el, &tlst);
		if (*input != 0)
		{
			add_history(input);
			tlst = tokenize(input, el);
			if (tlst)
				evaluate_cmdline(&tlst, &el);
		}
		free(input);
	}
	return (0);
}

static void	init_shell(t_envlst **el, t_termios	*old_settings, t_toklst **tl, \
		char **envp)
{
	*el = parse_env(envp);
	envlst_add_back(el, envlst_new("?", "0"));
	signal_setup(signal_handler);
	term_setup(old_settings);
	*tl = NULL;
}

static void	evaluate_cmdline(t_toklst **tl, t_envlst **el)
{
	// t_cmdlst	*cl;
	// char		*status_str;
	// int			status_int;
	//
	// cl = parse_tokenlist(*tl);
	// status_int = exec_cmd(cl, el);
	// status_str = ft_itoa(status_int);
	// set_env_entry("?", status_str, el);
	// free(status_str);
	// cmdlst_clear(&cl);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 14:58:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		set_exit_status_envvar(char *status_str, t_envlst **el);
static void	cleanup_and_exit(t_termios *old_settings, t_envlst **el, \
		t_toklst **tl);
static int	evaluate_cmdline(t_toklst **tl, t_envlst **el);
static void	init_shell(t_envlst **el, t_termios	*old_settings, t_toklst **tl, \
		char **envp);

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings;
	char		*input;
	t_toklst	*tlst;
	t_envlst	*el;
	int			exit_flag;

	(void)ac, (void)av;
	exit_flag = 0;
	init_shell(&el, &old_settings, &tlst, envp);
	while (1)
	{
		if (!exit_flag)
			read_prompt(&input, PROMPT);
		if (!input || exit_flag)
			cleanup_and_exit(&old_settings, &el, &tlst);
		if (*input != 0)
		{
			add_history(input);
			tlst = tokenize(input, &el);
			if (tlst)
				exit_flag = evaluate_cmdline(&tlst, &el);
		}
		free(input);
	}
	return (0);
}

static void	init_shell(t_envlst **el, t_termios	*old_settings, t_toklst **tl,
		char **envp)
{
	char	*shlvl_cnt;
	char	*pwd;

	*el = parse_env(envp);
	set_env_entry("?", "0", el);
	if (get_env_entry_by_name("SHLVL", *el))
	{
		shlvl_cnt = ft_itoa(ft_atoi(get_env_entry_by_name("SHLVL", *el)->value) + 1);
		set_env_entry("SHLVL", shlvl_cnt, el);
		free(shlvl_cnt);
	}
	else
		set_env_entry("SHLVL", "1", el);
	set_env_entry("OLDPWD", NULL, el);
	pwd = ft_calloc(1024, sizeof(char));
	getcwd(pwd, 500);
	set_env_entry("PWD", pwd, el);
	free(pwd);
	signal_setup(sigint_handler);
	term_setup(old_settings);
	*tl = NULL;
}

static int	evaluate_cmdline(t_toklst **tl, t_envlst **el)
{
	t_cmdlst	*cl;
	char		*status_str;
	int			status_int;
	int			exit_flag;

	cl = parse_tokenlist(tl);
	heredoc(cl, *el);
	status_int = exec_cmd(cl, el);
	if (status_int == 126 || status_int == 127)
		status_str = ft_itoa(status_int);
	else if (ft_wifsignaled(status_int))
	{
		status_str = ft_itoa(status_int + 128);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else
		status_str = ft_itoa(ft_wexitstatus(status_int));
	set_exit_status_envvar(status_str, el);
	heredoc_cleanup(cl);
	exit_flag = cl->exit_flag;
	cmdlst_clear(&cl);
	toklst_clear(tl);
	return (exit_flag);
}

static void	cleanup_and_exit(t_termios *old_settings, t_envlst **el, \
		t_toklst **tl)
{
	int	status;

	status = ft_atoi(get_env_entry_by_name("?", *el)->value);
	if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	tcsetattr(STDIN_FILENO, TCSANOW, old_settings);
	rl_clear_history();
	envlst_clear(el);
	if (*tl)
		toklst_clear(tl);
	exit(status);
}

/* Read the prompt. Either via readline when input is coming from terminal in
 * interactive mode, or using get_next_line when input is coming in
 * non-interactive mode, f.ex. during testing. */
void	read_prompt(char **input, char *prompt)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		*input = readline(prompt);
	else
	{
		line = get_next_line(STDIN_FILENO);
		*input = ft_strtrim(line, "\n");
		free(line);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 11:19:55 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void				set_exit_status_envvar(char *status_str, t_envlst **el);
static int			evaluate_cmdline(t_ministruct *mini);
static void			cleanup_and_exit(t_termios *old_settings, t_envlst **el,
						t_toklst **tl);
static t_ministruct	*init_shell(t_termios	*old_settings, char **envp, int ac,
						char **av);

int	main(int ac, char **av, char **envp)
{
	t_termios		old_settings;
	t_ministruct	*mini;
	char			*input;
	int				exit_flag;

	exit_flag = 0;
	mini = init_shell(&old_settings, envp, ac, av);
	while (1)
	{
		if (!exit_flag)
			read_prompt(&input, PROMPT, *mini);
		if (!input || exit_flag)
			cleanup_and_exit(&old_settings, &mini->el, &mini->tl);
		if (*input != 0)
		{
			add_history(input);
			mini->tl = tokenize(input, &mini->el);
			if (mini->tl)
				exit_flag = evaluate_cmdline(mini);
		}
		free(input);
	}
	return (0);
}

static t_ministruct	*init_shell(t_termios	*old_settings, char **envp, int ac,
		char **av)
{
	t_ministruct	*mini;

	mini = malloc(sizeof(t_ministruct));
	nullcheck(mini, "init_shell()");
	mini->el = parse_env(envp);
	init_shell_vars(&mini->el);
	mini->tl = NULL;
	mini->ac = ac;
	mini->av = av;
	if (ac == 2 && !ft_strcmp(av[1], "-s"))
		mini->script_mode = 1;
	else
		mini->script_mode = 0;
	if (isatty(STDIN_FILENO))
	{
		signal_setup(sigint_handler);
		term_setup(old_settings);
	}
	else
		signal(SIGINT, SIG_DFL);
	return (mini);
}

static int	evaluate_cmdline(t_ministruct *mini)
{
	t_cmdlst	*cl;
	char		*status_str;
	int			status_int;
	int			exit_flag;

	cl = parse_tokenlist(&mini->tl);
	heredoc(cl, mini);
	status_int = exec_cmd(cl, &mini->el);
	if (status_int == 126 || status_int == 127)
		status_str = ft_itoa(status_int);
	else if (ft_wifsignaled(status_int))
	{
		status_str = ft_itoa(status_int + 128);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	else
		status_str = ft_itoa(ft_wexitstatus(status_int));
	set_exit_status_envvar(status_str, &mini->el);
	heredoc_cleanup(cl);
	exit_flag = cl->exit_flag;
	cmdlst_clear(&cl);
	toklst_clear(&mini->tl);
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

/**
 *
 * Read the prompt.
 *
 * If stdout and stdin are normal tty devices, then show our fancy prompt and
 * use readline to read user input. If stdin isn't a tty or the `-s` command
 * line param was given to minshell, then show no prompt and just read stuff
 * using gnl. 3rd variant - the `./minishell | ./minishell` situation we print
 * hackily the prompt to stderr as this is not redirected to the pipe, and
 * handle input over gnl manually.
 * */
void	read_prompt(char **input, char *prompt, t_ministruct mini)
{
	char	*line;

	if (mini.script_mode || !isatty(STDIN_FILENO))
	{
		line = get_next_line(STDIN_FILENO);
		*input = ft_strtrim(line, "\n");
		free(line);
	}
	else if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		*input = readline(prompt);
	else if (isatty(STDIN_FILENO))
	{
		ft_dprintf(STDERR_FILENO, prompt);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			ft_dprintf(STDERR_FILENO, "\nexit\n");
			*input = NULL;
			return ;
		}
		*input = ft_strtrim(line, "\n");
		free(line);
	}
}

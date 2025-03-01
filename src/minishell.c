/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 08:34:30 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void				set_exit_status_envvar(char *status_str, t_envlst **el);
static int			evaluate_cmdline(t_ministruct *mini);
static void			cleanup_and_exit(t_ministruct *mini);
static t_ministruct	*init_shell(t_termios *old_settings, char **envp, int ac,
						char **av);

/**
 * Minishell main.
 *
 * 1) Init the shell, setup some data structs like the env linked list inside
 * the main control t_ministruct. Also signal setup & term setup is done.
 * 2) If nothing went wrong (mini != NULL) loop over prompt reading and prompt
 * evaluation.
 *
 * The exit_flag can be set by the exit builtin and ensures cleanup before
 * exiting the shell.
 */
int	main(int ac, char **av, char **envp)
{
	t_termios		old_settings;
	t_ministruct	*mini;
	char			*input;
	int				exit_flag;

	exit_flag = 0;
	mini = init_shell(&old_settings, envp, ac, av);
	while (mini != NULL)
	{
		if (!exit_flag)
			read_prompt(&input, PROMPT);
		if (!input || exit_flag)
			cleanup_and_exit(mini);
		if (*input != 0)
		{
			if (mini->stdin_istty)
				add_history(input);
			mini->tl = tokenize(input, &mini->el);
			if (mini->tl)
				exit_flag = evaluate_cmdline(mini);
		}
		free(input);
	}
	return (1);
}

/**
 * Initialize the Minishell.
 *
 * Returns the t_ministruct. Signal & term setup is also done in here. The only
 * 2 ways this can fail (return NULL) is either if malloc fails or the signal
 * setting in signal setups fails. Both very unlikekly.
 */
static t_ministruct	*init_shell(t_termios *old_settings, char **envp, int ac,
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
	mini->stdin_istty = isatty(STDIN_FILENO);
	mini->term = old_settings;
	if (mini->stdin_istty)
	{
		if (signal_setup(minish_sighandler) == -1)
			return (free(mini), NULL);
		term_setup(old_settings);
	}
	else
		if (signal(SIGINT, SIG_DFL) == SIG_ERR)
			return (free(mini), NULL);
	return (mini);
}

/**
 * The main cmdline evalution routine.
 *
 * The outer heredoc logic is also implemented in here. Meaning: If heredoc is
 * canceled with ctrl-c there will be no execution done.
 * 
 * The exit status is adjusted to correspond bash's. The most hacky part is
 * where we get the status == 42 from exec_cmd. This is the case if some early
 * pipe cmd failed due to signaling and we want an additional newline to be
 * printed.
 *
 * Returns the value of exit_flag in order to set it in main.
 */
static int	evaluate_cmdline(t_ministruct *mini)
{
	t_cmdlst	*cl;
	char		*status_str;
	int			status_int;
	int			exit_flag;

	cl = parse_tokenlist(&mini->tl);
	if (heredoc(cl, mini) != 2)
	{
		status_int = exec_cmd(cl, &mini->el);
		if (status_int == 126 || status_int == 127)
			status_str = ft_itoa(status_int);
		else if (ft_wifsignaled(status_int))
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			status_str = ft_itoa(!(status_int == 42) * (status_int + 128));
		}
		else
			status_str = ft_itoa(ft_wexitstatus(status_int));
		set_exit_status_envvar(status_str, &mini->el);
	}
	heredoc_cleanup(cl);
	exit_flag = cl->exit_flag;
	cmdlst_clear(&cl);
	toklst_clear(&mini->tl);
	return (exit_flag);
}

/* Well, cleanup. */
static void	cleanup_and_exit(t_ministruct *mini)
{
	int	status;

	status = ft_atoi(get_env_entry_by_name("?", mini->el)->value);
	if (mini->stdin_istty)
		ft_printf("exit\n");
	if (mini->stdin_istty)
		tcsetattr(STDIN_FILENO, TCSANOW, mini->term);
	rl_clear_history();
	envlst_clear(&mini->el);
	if (mini->tl)
		toklst_clear(&mini->tl);
	free(mini);
	exit(status);
}

/**
 *
 * Read the prompt.
 *
 * If stdout and stdin are normal tty devices, then show our fancy prompt and
 * use readline to read user input. If stdin isn't a tty show no prompt and just
 * read stuff using gnl. 3rd variant - the `./minishell | ./minishell` situation
 * we print hackily the prompt to stderr as this is not redirected to the pipe,
 * and handle input over gnl manually =;)
 * */
void	read_prompt(char **input, char *prompt)
{
	char	*line;

	if (!isatty(STDIN_FILENO))
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:46:50 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 13:32:30 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_toklst_del(t_toklst **tlst);

int	main(int ac, char **av, char **envp)
{
	t_termios	old_settings; char		*input;
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
			ft_printf(RED "<< DEBUG >> toklist after lvl3:\n" RST);
			tokenize_lvl3(&tlst);
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

void	test_toklst_del(t_toklst **tlst)
{
	t_toklst	*vic;
	t_toklst	*tmp;

	tmp = *tlst;
	while (tmp)
	{
		if (tmp->token->type == TOK_VAR_SYM)
			vic = tmp;
		tmp = tmp->next;
	}
	toklst_del(tlst, vic);
	ft_printf(RED "<< DEBUG >> toklst after del:\n" RST);
	print_toklst(*tlst);
}

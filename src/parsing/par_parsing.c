/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/21 09:50:29 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_heredoc(t_toklst **toklst);
static int	get_max_argnum(t_toklst *tl);

/* Parse tokenlist into cmdlist. */
t_cmdlst	*parse_tokenlist(t_toklst *toklst)
{
	t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;
	int			maxargs;

	if (!toklst)
		return (NULL);
	remove_heredoc(&toklst);
	maxargs = get_max_argnum(toklst);
	cmd = cmdlst_new(NULL, maxargs);
	cur_cmd = cmd;
	while (toklst)
	{
		parse_command(&toklst, &cmd, &cur_cmd, maxargs);
		parse_args(&toklst, &cur_cmd);
		parse_builtin(&toklst, &cmd, &cur_cmd, maxargs);
		parse_pipe(&toklst, &cmd, &cur_cmd, maxargs);
		parse_rout(&toklst, cur_cmd);
		parse_rin(&toklst, cur_cmd);
		parse_heredoc(&toklst, cur_cmd);
	}

#ifdef DEBUG
	ft_printf(RED "<< DEBUG >> cmdlst:\n" RST);
	print_cmdlst(cmd);
#endif

	return (cmd);
}

/* Print command structure (for debugging) */
void	print_cmdlst(t_cmdlst *cmd)
{
	int	i;

	while (cmd)
	{
		ft_printf("-- cmd start --\nCommand: %s\nArguments: ", cmd->cmd);
		i = 0;
		while (++i < cmd->arg_count)
			ft_printf("%s ", cmd->args[i]);
		ft_printf("\n");
		if (cmd->outfiles)
		{
			ft_printf("Output redirects:\n", cmd->input_file);
			redirlst_print(cmd->outfiles);
		}
		if (cmd->input_file)
			ft_printf("Input redirected from: %s\n", cmd->input_file);
		if (cmd->is_builtin)
			ft_printf("Is builtin!\n");
		if (cmd->heredoc)
			ft_printf("HEREDOC: %s\n", cmd->heredoc);
		if (cmd->next)
			ft_printf("Piped to: %s\n", cmd->next->cmd);
		ft_printf("-- cmd end --\n");
		cmd = cmd->next;
	}
}

static void	remove_heredoc(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_HERE)
			toklst_remove_tok(toklst, &tl);
		else
			tl = tl->next;
	}
}

/* Get the maximum number of args per one cmd present in the current cmdline
 * input. */
static int	get_max_argnum(t_toklst *tl)
{
	int	argcnt;
	int	max;

	argcnt = 0;
	max = 0;
	while (tl)
	{
		if (tl->token->type == TOK_ARG)
			argcnt++;
		else if (tl->token->type == TOK_PIP)
		{
			if (argcnt > max)
			{
				max = argcnt;
				argcnt = 0;
			}
		}
		tl = tl->next;
	}
	if (argcnt > max)
		return (argcnt);
	return (max);
}

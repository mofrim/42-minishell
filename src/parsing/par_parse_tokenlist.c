/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_tokenlist.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 11:42:56 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_max_argnum(t_toklst *tl);
void		print_cmdlst(t_cmdlst *cmd);

/* Parse tokenlist into cmdlist. */
t_cmdlst	*parse_tokenlist(t_toklst **toklst)
{
	t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;
	t_toklst	*tl;
	int			maxargs;


	if (!*toklst)
		return (NULL);
	maxargs = get_max_argnum(*toklst);
	cmd = cmdlst_new(NULL, maxargs);
	cur_cmd = cmd;
	tl = *toklst;
	while (tl)
	{
		parse_command(&tl, &cmd, &cur_cmd, maxargs);
		parse_args(&tl, &cur_cmd);
		parse_builtin(&tl, &cmd, &cur_cmd, maxargs);
		parse_pipe(&tl, &cmd, &cur_cmd, maxargs);
		parse_rout(&tl, cur_cmd);
		parse_rin(&tl, cur_cmd);
		parse_heredoc(&tl, cur_cmd);
	}

#ifdef DEBUG
	ft_printf(RED "<< DEBUG >> cmdlst:\n" RST);
	print_cmdlst(cmd);
#endif

	return (cmd);
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

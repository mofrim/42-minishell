/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:29:40 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 12:02:53 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a pipe. */
void	parse_pipe(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd,
					int maxargs)
{
	if (*toklst && (*toklst)->next)
	{
		if ((*toklst)->token->type == TOK_PIP && \
				((*toklst)->next->token->type == TOK_CMD || \
				(*toklst)->next->token->type == TOK_BLTIN))
			*toklst = (*toklst)->next;
		else if ((*toklst)->token->type == TOK_PIP && \
				!is_redir_tok((*toklst)->token->type))
		{
			*cur_cmd = cmdlst_new(NULL, maxargs);
			cmdlst_add_back(cmd, *cur_cmd);
			*toklst = (*toklst)->next;
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:29:40 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/02 15:11:42 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Parse a pipe sybmbol.
 *
 * The logic here is that the TOK_PIPEs are the seperators of our cmdlst.
 * Meaning: there will never be a TOK_PIPE that is not starting a new cmd to be
 * parsed! Meaning: Always allocate & add_back & not forget to increment
 * cmd_count (!!!) a new cdmdlst member with cmd set to NULL. It is that
 * simple, bc any cases like `| echo "bla"` are already filtered out by our
 * tokenization process.
 */
void	parse_pipe(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd,
					int maxargs)
{
	if (*toklst && (*toklst)->next && (*toklst)->token->type == TOK_PIP)
	{
		*cur_cmd = cmdlst_new(NULL, maxargs);
		cmdlst_add_back(cmd, *cur_cmd);
		(*cmd)->cmd_count++;
		*toklst = (*toklst)->next;
	}
}

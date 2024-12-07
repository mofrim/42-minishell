/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing_helpers1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:29:40 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/07 19:48:53 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a pipe. */
void	parse_pipe(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd, \
		int maxargs)
{
	if (*toklst && (*toklst)->next)
	{
		if ((*toklst)->token->type == TOK_PIP && \
				(*toklst)->next->token->type == TOK_CMD)
			*toklst = (*toklst)->next;
		else if ((*toklst)->token->type == TOK_PIP && \
				((*toklst)->next->token->type == TOK_RIN || \
				(*toklst)->next->token->type == TOK_ROUT))
		{
			*cur_cmd = cmdlst_new(NULL, maxargs);
			cmdlst_add_back(cmd, *cur_cmd);
			*toklst = (*toklst)->next;
		}
	}
}

/* Parse a output redirect. */
void	parse_rout(t_toklst **toklst, t_cmdlst *cur_cmd)
{
	if (*toklst && ((*toklst)->token->type == TOK_ROUT || \
			(*toklst)->token->type == TOK_ROUTA))
	{
		cur_cmd->output_file = ft_strdup((*toklst)->next->token->value);
		if ((*toklst)->token->type == TOK_ROUTA)
			cur_cmd->append = 1;
		(*toklst) = (*toklst)->next->next;
	}
}

/* Parse a input redirect. If there was already an input_file set free it an
 * replace with new one found. */
void	parse_rin(t_toklst **toklst, t_cmdlst *cur_cmd)
{
	if (*toklst && (*toklst)->next && (*toklst)->token->type == TOK_RIN)
	{

		if (cur_cmd->input_file)
			free(cur_cmd->input_file);
		cur_cmd->input_file = ft_strdup((*toklst)->next->token->value);
		(*toklst) = (*toklst)->next->next;
	}
}

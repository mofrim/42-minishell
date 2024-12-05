/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing_helpers1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:29:40 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 12:03:37 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a pipe. */
void	parse_pipe(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
{
	if (*toklst && (*toklst)->next)
	{
		if ((*toklst)->token->type == TOK_PIP && \
				(*toklst)->next->token->type == TOK_CMD)
			*toklst = (*toklst)->next;
		if ((*toklst)->token->type == TOK_PIP && \
				((*toklst)->next->token->type == TOK_RIN || \
				(*toklst)->next->token->type == TOK_ROUT))
		{
			*cur_cmd = cmdlst_new(NULL);
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

/* Parse a input redirect. */
void	parse_rin(t_toklst **toklst, t_cmdlst *cur_cmd)
{
	if (*toklst && (*toklst)->next && (*toklst)->token->type == TOK_RIN)
	{
		cur_cmd->input_file = ft_strdup((*toklst)->next->token->value);
		(*toklst) = (*toklst)->next->next;
	}
}

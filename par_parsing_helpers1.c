/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing_helpers1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:29:40 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/03 21:27:21 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a TOK_CMD and following TOK_ARGs. If after that no tokens are left
 * return 0. Otherwise return 1. NULL-terminate the args-array in any case
 * because execve() needs this! Reminder: arg_count is initialized to 1, so this
 * will also work if there is only a cmd without args.*/
void	parse_command(t_tokenlist **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
{
	if (*toklst && (*toklst)->token->type == TOK_CMD)
	{
		if (*cur_cmd && (*cur_cmd)->cmd)
		{
			*cur_cmd = cmdlst_new((*toklst)->token->value);
			cmdlst_add_back(cmd, *cur_cmd);
		}
		else
			(*cur_cmd)->cmd = ft_strdup((*toklst)->token->value);
		*toklst = (*toklst)->next;
		while (*toklst && (*toklst)->token->type == TOK_ARG)
		{
			(*cur_cmd)->args[(*cur_cmd)->arg_count++] \
				= ft_strdup((*toklst)->token->value);
			*toklst = (*toklst)->next;
		}
		(*cur_cmd)->args[(*cur_cmd)->arg_count] = NULL;
	}
}

void	parse_pipe(t_tokenlist **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
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

void	parse_rout(t_tokenlist **toklst, t_cmdlst *cur_cmd)
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

void	parse_rin(t_tokenlist **toklst, t_cmdlst *cur_cmd)
{
	if (*toklst && (*toklst)->next && (*toklst)->token->type == TOK_RIN)
	{
		cur_cmd->input_file = ft_strdup((*toklst)->next->token->value);
		(*toklst) = (*toklst)->next->next;
	}
}

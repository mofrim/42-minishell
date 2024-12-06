/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_cmd_bltin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 11:10:41 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 12:37:17 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_args(t_toklst **toklst, t_cmdlst **cur_cmd);

/* Parse a TOK_CMD and following TOK_ARGs. NULL-terminate the args-array in any
 * case because execve() needs this! Reminder: arg_count is initialized to 1, so
 * this will also work if there is only a cmd without args.*/
void	parse_command(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
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
		(*cur_cmd)->args[0] = ft_strdup((*cur_cmd)->cmd);
		*toklst = (*toklst)->next;
		parse_args(toklst, cur_cmd);
	}
}

void	parse_builtin(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
{
	if (*toklst && (*toklst)->token->type == TOK_BLTIN)
	{
		if (*cur_cmd && (*cur_cmd)->cmd)
		{
			*cur_cmd = cmdlst_new((*toklst)->token->value);
			cmdlst_add_back(cmd, *cur_cmd);
		}
		else
			(*cur_cmd)->cmd = ft_strdup((*toklst)->token->value);
		(*cur_cmd)->args[0] = ft_strdup((*cur_cmd)->cmd);
		(*cur_cmd)->is_builtin = 1;
		*toklst = (*toklst)->next;
		parse_args(toklst, cur_cmd);
	}
}

static void	parse_args(t_toklst **toklst, t_cmdlst **cur_cmd)
{
	while (*toklst && (*toklst)->token->type == TOK_ARG)
	{
		(*cur_cmd)->args[(*cur_cmd)->arg_count++] \
			= ft_strdup((*toklst)->token->value);
		*toklst = (*toklst)->next;
	}
	(*cur_cmd)->args[(*cur_cmd)->arg_count] = NULL;
}

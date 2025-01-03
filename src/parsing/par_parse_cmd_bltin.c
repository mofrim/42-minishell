/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_cmd_bltin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 11:10:41 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/18 22:38:14 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse a TOK_CMD.*/
void	parse_command(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd, \
		int maxargs)
{
	if (*toklst && (*toklst)->token->type == TOK_CMD)
	{
		if (*cur_cmd && (*cur_cmd)->cmd)
		{
			*cur_cmd = cmdlst_new((*toklst)->token->value, maxargs);
			cmdlst_add_back(cmd, *cur_cmd);
			(*cmd)->cmd_count++;
		}
		else
			(*cur_cmd)->cmd = ft_strdup((*toklst)->token->value);
		(*cur_cmd)->args[0] = ft_strdup((*cur_cmd)->cmd);
		*toklst = (*toklst)->next;
	}
}

/* Parse a builtin cmd. */
void	parse_builtin(t_toklst **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd, \
		int maxargs)
{
	if (*toklst && (*toklst)->token->type == TOK_BLTIN)
	{
		if (*cur_cmd && (*cur_cmd)->cmd)
		{
			*cur_cmd = cmdlst_new((*toklst)->token->value, maxargs);
			cmdlst_add_back(cmd, *cur_cmd);
			(*cmd)->cmd_count++;
		}
		else
			(*cur_cmd)->cmd = ft_strdup((*toklst)->token->value);
		(*cur_cmd)->args[0] = ft_strdup((*cur_cmd)->cmd);
		(*cur_cmd)->is_builtin = 1;
		*toklst = (*toklst)->next;
	}
}

/* Parse any TOK_ARG. */
void	parse_args(t_toklst **toklst, t_cmdlst **cur_cmd)
{
	while (*toklst && (*toklst)->token->type == TOK_ARG)
	{
		(*cur_cmd)->args[(*cur_cmd)->arg_count++] \
			= ft_strdup((*toklst)->token->value);
		*toklst = (*toklst)->next;
	}
	(*cur_cmd)->args[(*cur_cmd)->arg_count] = NULL;
}

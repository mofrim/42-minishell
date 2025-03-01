/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_quot.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:47:57 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 15:53:41 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_tok_emptyquot(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->length - cl->pos > 1 && \
				(!ft_strncmp(&cl->input[cl->pos], "\"\"", 2) || \
				!ft_strncmp(&cl->input[cl->pos], "''", 2)))
		{
			tok->type = TOK_QWORD;
			tok->value = ft_strdup("");
			nullcheck(tok->value, "get_tok_emptyquot()");
			cl->pos += 2;
			*tok_found = 1;
		}
	}
}

void	get_tok_quot(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '"' && !cl->squot_flag)
		{
			tok->type = TOK_DQUOT;
			tok->value = ft_strdup("\"");
			nullcheck(tok->value, "get_tok_quot()");
			cl->pos++;
			*tok_found = 1;
			cl->dquot_flag = !cl->dquot_flag;
		}
		else if (cl->input[cl->pos] == '\'' && !cl->dquot_flag)
		{
			tok->type = TOK_SQUOT;
			tok->value = ft_strdup("\'");
			nullcheck(tok->value, "get_tok_quot()");
			cl->pos++;
			*tok_found = 1;
			cl->squot_flag = !cl->squot_flag;
		}
	}
}

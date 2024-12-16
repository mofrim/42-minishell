/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_helpers1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:45:01 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 20:50:00 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(t_cmdline *cl)
{
	while (cl->pos < cl->length && \
			ft_isspace(cl->input[cl->pos]))
		cl->pos++;
}

void	get_tok_pipe(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '|')
		{
			tok->type = TOK_PIP;
			tok->value = ft_strdup("|");
			nullcheck(tok->value, "get_tok_pipe()");
			cl->pos++;
			*tok_found = 1;
		}
	}
}

void	get_tok_rin(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '<')
		{
			tok->type = TOK_RIN;
			tok->value = ft_strdup("<");
			nullcheck(tok->value, "get_tok_rin()");
			cl->pos++;
			*tok_found = 1;
		}
	}
}

void	get_tok_here(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int	pos;

	if (!*tok_found)
	{
		pos = cl->pos;
		if (pos < cl->length - 1)
		{
			if (cl->input[pos] == '<' && cl->input[pos + 1] == '<')
			{
				tok->type = TOK_HERE;
				tok->value = ft_strdup("<<");
				nullcheck(tok->value, "get_tok_here()");
				cl->pos += 2;
				*tok_found = 1;
			}
		}
	}
}

void	get_tok_and(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '&')
		{
			tok->type = TOK_AND;
			tok->value = ft_strdup("&");
			nullcheck(tok->value, "get_tok_and()");
			cl->pos++;
			*tok_found = 1;
		}
	}
}

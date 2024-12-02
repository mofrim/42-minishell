/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_helpers2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:47:57 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/02 17:41:39 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_tok_quot(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '"')
		{
			tok->type = TOK_DQUOT;
			tok->value = ft_strdup("\"");
			nullcheck(tok->value, "get_tok_quot()");
			cl->pos++;
			*tok_found = 1;
			cl->dquot_flag = !cl->dquot_flag;
		}
		if (cl->input[cl->pos] == '\'')
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

void	get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		start;
	int		length;
	char	*word;

	if (!*tok_found)
	{
		start = cl->pos;
		while (cl->pos < cl->length && \
				!ft_isspace(cl->input[cl->pos]) && \
				cl->input[cl->pos] != '|' && \
				cl->input[cl->pos] != '>' && \
				cl->input[cl->pos] != '<' && \
				cl->input[cl->pos] != '"' && \
				cl->input[cl->pos] != '$' && \
				cl->input[cl->pos] != '\'')
			cl->pos++;
		length = cl->pos - start;
		word = malloc(length + 1);
		nullcheck(word, "get_tok_word()");
		ft_strlcpy(word, &cl->input[start], length + 1);
		tok->value = word;
		tok->type = TOK_WORD;
		*tok_found = 1;
	}
}

void	get_tok_squotword(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		start;
	int		length;
	char	*word;

	if (!*tok_found)
	{
		if (cl->squot_flag)
		{
			start = cl->pos;
			while (cl->pos < cl->length && cl->input[cl->pos] != '\'')
				cl->pos++;
			length = cl->pos - start;
			word = malloc(length + 1);
			nullcheck(tok->value, "get_tok_squotword()");
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_SQUOT_TXT;
			*tok_found = 1;
		}
	}
}

void	get_tok_dquotword(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		start;
	int		length;
	char	*word;

	if (!*tok_found)
	{
		if (cl->dquot_flag)
		{
			start = cl->pos;
			while (cl->pos < cl->length && cl->input[cl->pos] != '"' && \
					cl->input[cl->pos] != '$')
				cl->pos++;
			length = cl->pos - start;
			word = malloc(length + 1);
			nullcheck(word, "get_tok_dquotword()");
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_DQUOT_TXT;
			*tok_found = 1;
		}
	}
}

void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found && !cl->squot_flag && !cl->var_flag)
	{
		if (cl->input[cl->pos] == '$')
		{
			tok->type = TOK_VAR_SYM;
			tok->value = ft_strdup("$");
			nullcheck(tok->value, "get_tok_var()");
			cl->pos++;
			cl->var_flag = 1;
			*tok_found = 1;
		}
	}
	if (!*tok_found && cl->var_flag)
	{
		get_tok_word(tok, cl, tok_found);
		tok->type = TOK_VAR_NAME;
		cl->var_flag = 0;
	}
}

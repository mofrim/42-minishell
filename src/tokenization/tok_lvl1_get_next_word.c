/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_word.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:01:20 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 10:32:49 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator_or_special_char(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '&' || c == '"' || \
			c == '$' || c == '\'')
		return (1);
	return (0);
}

void	get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		start;
	int		length;
	char	*word;

	if (!*tok_found)
	{
		start = cl->pos;
		while (cl->pos < cl->length && !ft_isspace(cl->input[cl->pos]) && \
				!is_operator_or_special_char(cl->input[cl->pos]))
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
			nullcheck(word, "get_tok_squotword()");
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_QWORD;
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
			word = ft_calloc(sizeof(char), length + 1);
			nullcheck(word, "get_tok_dquotword()");
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_QWORD;
			*tok_found = 1;
		}
	}
}

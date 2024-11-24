/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_get_next_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:55:17 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/24 23:43:24 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Skip whitespace */
static void	skip_whitespace(t_cmdline *cl)
{
	while (cl->pos < cl->length && \
			ft_isspace(cl->input[cl->pos]))
		cl->pos++;
}

static void	get_tok_pipe(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '|')
		{
			tok->type = TOK_PIP;
			tok->value = ft_strdup("|"); // FIXME handle malloc
			cl->pos++;
			*tok_found = 1;
		}
	}
}

static void	get_tok_rin(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '<')
		{
			tok->type = TOK_RIN;
			tok->value = ft_strdup("<"); // FIXME handle malloc
			cl->pos++;
			*tok_found = 1;
		}
	}
}

static void	get_tok_rout(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int	pos;

	if (!*tok_found)
	{
		pos = cl->pos;
		if (cl->pos < cl->length - 1)
		{
			if (cl->input[pos] == '>' && cl->input[pos + 1] == '>')
			{
				tok->type = TOK_ROUTA;
				tok->value = ft_strdup(">>"); // FIXME handle malloc
				cl->pos += 2;
				*tok_found = 1;
			}
			if (cl->input[pos] == '>' && cl->input[pos + 1] != '>')
			{
				tok->type = TOK_ROUT;
				tok->value = ft_strdup(">"); // FIXME handle malloc
				cl->pos++;
				*tok_found = 1;
			}
		}
	}
}

static void	get_tok_here(t_token *tok, t_cmdline *cl, int *tok_found)
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
				tok->value = ft_strdup("<<"); // FIXME handle malloc
				cl->pos += 2;
				*tok_found = 1;
			}
		}
	}
}

static void	get_tok_quot(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found)
	{
		if (cl->input[cl->pos] == '"')
		{
			tok->type = TOK_DQUOT;
			tok->value = ft_strdup("\""); // FIXME handle malloc
			cl->pos++;
			*tok_found = 1;
			cl->dquot_flag = !cl->dquot_flag;
		}
		if (cl->input[cl->pos] == '\'')
		{
			tok->type = TOK_SQUOT;
			tok->value = ft_strdup("\'"); // FIXME handle malloc
			cl->pos++;
			*tok_found = 1;
			cl->squot_flag = !cl->squot_flag;
		}
	}
}

static void	get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found)
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
		word = malloc(length + 1); // FIXME handle malloc
		ft_strlcpy(word, &cl->input[start], length + 1);
		tok->value = word;
		tok->type = TOK_WORD;
		*tok_found = 1;
	}
}

static void	get_tok_squotword(t_token *tok, t_cmdline *cl, int *tok_found)
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
			word = malloc(length + 1); // FIXME handle malloc
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_SQUOT_TXT;
			*tok_found = 1;
		}
	}
}

static void	get_tok_dquotword(t_token *tok, t_cmdline *cl, int *tok_found)
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
			word = malloc(length + 1); // FIXME handle malloc
			ft_strlcpy(word, &cl->input[start], length + 1);
			tok->value = word;
			tok->type = TOK_DQUOT_TXT;
			*tok_found = 1;
		}
	}
}

static void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found && !cl->squot_flag && !cl->var_flag)
	{
		if (cl->input[cl->pos] == '$')
		{
			tok->type = TOK_VAR_SYM;
			tok->value = ft_strdup("$"); // FIXME handle malloc
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

static void	get_tok_unquoted(t_token *tok, t_cmdline *cl, int *tok_found)
{
	skip_whitespace(cl);
	get_tok_quot(tok, cl, tok_found);
	get_tok_var(tok, cl, tok_found);
	get_tok_pipe(tok, cl, tok_found);
	get_tok_rout(tok, cl, tok_found);
	get_tok_here(tok, cl, tok_found);
	get_tok_rin(tok, cl, tok_found);
	get_tok_word(tok, cl, tok_found);
}

static void	get_tok_quoted(t_token *tok, t_cmdline *cl, int *tok_found)
{
	get_tok_quot(tok, cl, tok_found);
	get_tok_var(tok, cl, tok_found);
	get_tok_squotword(tok, cl, tok_found);
	get_tok_dquotword(tok, cl, tok_found);
}

t_token	*get_next_token(t_cmdline *cl)
{
	t_token	*token;
	int		tok_found;

	token = malloc(sizeof(t_token)); // FIXME handle malloc
	if (cl->pos >= cl->length)
		return (NULL);
	tok_found = 0;
	while (cl->pos < cl->length && !tok_found)
	{
		if (!cl->dquot_flag && !cl->squot_flag)
			get_tok_unquoted(token, cl, &tok_found);
		else
			get_tok_quoted(token, cl, &tok_found);
	}
	return (token);
}

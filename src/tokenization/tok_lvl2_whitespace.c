/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:08:06 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 22:28:10 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static int	is_mergable_token(t_token tok);
static void	lvl2_remove_white_tokens(t_toklst **toklst);
static void	lvl2_apply_whitespace_tokenization(t_token *prev, t_token *cur,
				t_token *next);

/**
 * Outer loop for lvl2 whitespace tokenization.
 *
 * First we split possible TOK_WORD/TOK_CMD/TOK_BLTINs that contain whitspaces
 * and insert them into our toklst. This needs to be done because envvars with
 * values like "bla  blub" should be treated as TOK_WHITE seperated TOK_WORDs
 * from here... unless they were quoted! Further explanation see comment on
 * lvl2_apply_whitespace_tokenization.
 */
void	lvl2_whitespace_tokenization(t_toklst **toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;

	split_tokens_with_whitespaces(toklst);
	tl = *toklst;
	prev = NULL;
	cur = tl->token;
	while (tl->next)
	{
		next = tl->next->token;
		lvl2_apply_whitespace_tokenization(prev, cur, next);
		if (cur->type != TOK_NULL)
			prev = cur;
		cur = next;
		tl = tl->next;
	}
	lvl2_remove_white_tokens(toklst);
}

/* If at this lvl we find some TOK_CMD,BLTIN,ARG or WORD which is *not* followed
 * by a TOK_WHITE but directly tokens of the same kind, this means there was at
 * input cmdline no whitespace between them. In our current tokenization concept
 * this means: merge them all into the current token.
 * The `else if` handles the case where we have multiple mergable tokens in a
 * row.
 */
static void	lvl2_apply_whitespace_tokenization(t_token *prev, t_token *cur,
												t_token *next)
{
	char	*tmp;

	if (is_mergable_token(*cur) && is_mergable_token(*next))
	{
		tmp = cur->value;
		cur->value = ft_strjoin(cur->value, next->value);
		next->type = TOK_NULL;
		free(tmp);
	}
	else if (prev && cur->type == TOK_NULL && is_mergable_token(*prev) && \
			is_mergable_token(*next))
	{
		tmp = prev->value;
		prev->value = ft_strjoin(prev->value, next->value);
		next->type = TOK_NULL;
		free(tmp);
	}
}

/* Helper function for lvl2_apply_whitespace_tokenization. */
static int	is_mergable_token(t_token tok)
{
	if (tok.type == TOK_CMD || tok.type == TOK_BLTIN || tok.type == TOK_WORD \
			|| tok.type == TOK_ARG || tok.type == TOK_QWORD || \
			tok.type == TOK_VWORD)
		return (1);
	return (0);
}

/* Remove whitespace and NULLed tokens from last lvl. A token will be
 * TOK_NULLed if it was part of a non TOK_WHITE seperated token list. So all
 * members of that list will have been merged into the first member of the
 * list. */
void	lvl2_remove_white_tokens(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_WHITE)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_NULL)
			toklst_remove_tok(toklst, &tl);
		else
			if (tl)
				tl = tl->next;
	}
}

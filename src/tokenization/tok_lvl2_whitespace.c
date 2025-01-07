/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_whitespace.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:08:06 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/07 12:13:49 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

static void	lvl2_remove_white_tokens(t_toklst **toklst);
static void	lvl2_apply_whitespace_tokenization(t_token *prev, t_token *cur,
				t_token *next);

/* Outer loop for lvl2 whitespace tokenization. Further explanation see comment
 * on lvl2_apply_whitespace_tokenization.
 */
void	lvl2_whitespace_tokenization(t_toklst **toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;

	tl = *toklst;
	cur = tl->token;
	while (tl->next)
	{
		next = tl->next->token;
		lvl2_apply_whitespace_tokenization(prev, cur, next);
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
 */
static void	lvl2_apply_whitespace_tokenization(t_token *prev, t_token *cur,
												t_token *next)
{
	char	*tmp;

	(void)prev;
	if ((cur->type == TOK_CMD || cur->type == TOK_BLTIN || \
				cur->type == TOK_WORD || cur->type == TOK_ARG) && \
			(next->type == TOK_CMD || next->type == TOK_BLTIN \
			|| next->type == TOK_WORD || next->type == TOK_ARG))
	{
		tmp = cur->value;
		cur->value = ft_strjoin(cur->value, next->value);
		next->type = TOK_NULL;
		free(tmp);
	}
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
			tl = tl->next;
	}
}

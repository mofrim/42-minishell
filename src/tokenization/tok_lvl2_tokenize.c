/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:57:29 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 11:08:35 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The helper-functions. */
void		lvl2_whitespace_tokenization(t_toklst **toklst);
static void	lvl2_apply_tokenization(t_token *prev, t_token *cur, t_token *next);
static void	lvl2_remove_obsolete_tokens(t_toklst **toklst);

/**
 * Tokenization Level 2. 
 *
 * Goal is to classify all the TOK_WORD tokens and rule out some invalid syntax
 * like:
 * 	- "... < < ... "
 * 	- "... | <"
 * 	- "<< |"
 * 	- "<< <"
 * 	- "<< >"
 * 	- a lot more
 *
 * 	Rules:
 * 	1) after >, <, >>, and << there HAS to be a TOK_WORD!
 * 	2) VAR_NAMES cannot contain |, <, >, <<, >>, ", '
 * 	3) | cannot be the first token
 * 	4) ... and many more :)
 *
 * So in this level most of syntax checking is happening in `lvl2_check_toklst`.
 * Most errors are related to invalid metachar combinations like `> >`.
 */
int	tokenize_lvl2(t_toklst	**toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;

	lvl2_remove_obsolete_tokens(toklst);
	if (!*toklst)
		return (1);
	lvl2_whitespace_tokenization(toklst);
	if (lvl2_check_toklst(*toklst))
		return (2);
	tl = *toklst;
	cur = tl->token;
	if (cur->type == TOK_WORD || cur->type == TOK_VWORD)
		cur->type = is_cmd_or_builtin(cur->value);
	prev = NULL;
	while (tl->next)
	{
		next = tl->next->token;
		lvl2_apply_tokenization(prev, cur, next);
		prev = cur;
		cur = next;
		tl = tl->next;
	}
	return (0);
}

static void	lvl2_apply_tokenization(t_token *prev, t_token *cur, t_token *next)
{
	if (cur->type == TOK_RIN0 && is_word_tok(next->type))
		next->type = TOK_IF;
	else if (is_rout_tok(cur->type) && is_word_tok(next->type))
		next->type = TOK_OF;
	else if ((cur->type == TOK_IF || cur->type == TOK_OF || \
	cur->type == TOK_PIP || cur->type == TOK_ROUTA_FDTO || \
	cur->type == TOK_ROUT3_FDTO) && is_word_tok(next->type))
		next->type = is_cmd_or_builtin(next->value);
	else if (cur->type == TOK_CMD && is_word_tok(next->type))
		next->type = TOK_ARG;
	else if (cur->type == TOK_BLTIN && is_word_tok(next->type))
		next->type = TOK_ARG;
	else if (cur->type == TOK_ARG && is_word_tok(next->type))
		next->type = TOK_ARG;
	else if (cur->type == TOK_HERE && is_word_tok(next->type))
		next->type = TOK_HERE_DLIM;
	apply_redir_tokenization(prev, cur, next);
}

/* Remove some obsolete tokens in lvl2. */
void	lvl2_remove_obsolete_tokens(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_VAR_SYM)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_DQUOT)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_SQUOT)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_AND)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_VWORD && tl->token->value[0] == 0)
			toklst_remove_tok(toklst, &tl);
		else
			if (tl)
				tl = tl->next;
	}
}

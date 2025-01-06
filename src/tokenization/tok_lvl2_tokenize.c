/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:57:29 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 13:21:45 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* The helper-functions. */
static void	apply_lvl2_tokenization(t_token *prev, t_token *cur, t_token *next);
static void	lvl2_remove_obsolete_tokens(t_toklst **toklst);

/* Tokenization Level 2. Goal is to classify all the TOK_WORD tokens and rule
 * out some invalid syntax like
 *
 * 	- "... < < ... "
 * 	- "... | <"
 * 	- "<< |"
 * 	- "<< <"
 * 	- "<< >"
 * 	- a lot more
 *
 * 	Rulez:
 * 	1) after >, <, >>, and << there HAS to be a TOK_WORD!
 * 	2) VAR_NAMES cannot contain |, <, >, <<, >>, ", '
 * 	3) | cannot be the first token
 */
int	tokenize_lvl2(t_toklst	**toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;

	lvl2_remove_obsolete_tokens(toklst);
	if (!check_toklst_lvl2(*toklst))
		return (0);
	if (!*toklst)
		return (0);
	tl = *toklst;
	cur = tl->token;
	if (cur->type == TOK_WORD)
		cur->type = is_cmd_or_builtin(cur->value);
	prev = NULL;
	while (tl->next)
	{
		next = tl->next->token;
		apply_lvl2_tokenization(prev, cur, next);
		prev = cur;
		cur = next;
		tl = tl->next;
	}
	return (1);
}

static void	apply_lvl2_whitespace_tokenization(t_token *prev, t_token *cur,
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

static void	apply_lvl2_tokenization(t_token *prev, t_token *cur, t_token *next)
{
	apply_lvl2_whitespace_tokenization(prev, cur, next);
	if (cur->type == TOK_RIN0 && next->type == TOK_WORD)
		next->type = TOK_IF;
	else if (is_rout_tok(cur->type) && next->type == TOK_WORD)
		next->type = TOK_OF;
	else if ((cur->type == TOK_IF || cur->type == TOK_OF || \
	cur->type == TOK_PIP || cur->type == TOK_ROUTA_FDTO || \
	cur->type == TOK_ROUT3_FDTO) && next->type == TOK_WORD)
		next->type = is_cmd_or_builtin(next->value);
	else if (cur->type == TOK_CMD && (next->type == TOK_WORD))
		next->type = TOK_ARG;
	else if (cur->type == TOK_BLTIN && next->type == TOK_WORD)
		next->type = TOK_ARG;
	else if (cur->type == TOK_ARG && next->type == TOK_WORD)
		next->type = TOK_ARG;
	else if (cur->type == TOK_HERE && next->type == TOK_WORD)
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
		else
			tl = tl->next;
	}
}

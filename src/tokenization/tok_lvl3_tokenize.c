/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:24:38 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 11:42:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	lvl3_check_toklst(t_toklst *toklst);
static int	check_tok_lvl3(t_token *prev, t_token *cur, t_token *next);
static void	apply_lvl3_tokenization(t_token *cur, int *cmd_already);
static void	lvl3_remove_obsolete_tokens(t_toklst **toklst);

/*
 * Do the lvl3 tokenization.
 *
 * So far in lvl3 we have to fix some false tokenizations from lvl2 & correctly
 * classify TOK_CMD and TOK_BLTIN tokens. So this function scans through
 * the whole toklst, finds the first TOK_CMD/BLTIN sets the cmd_already flag.
 * After that if there is another TOK_CMD/BLTIN before the next pipe this is a
 * false classification. This token then needs to be and gets turned into
 * TOK_ARG.
 * Return 1 if anything goes wrong, 0 otherwise.
 */
int	tokenize_lvl3(t_toklst	**toklst)
{
	t_token		*cur;
	t_toklst	*tl;
	int			cmd_already;

	if (!*toklst)
		return (1);
	if (!lvl3_check_toklst(*toklst))
		return (1);
	tl = *toklst;
	cmd_already = 0;
	while (tl)
	{
		cur = tl->token;
		apply_lvl3_tokenization(cur, &cmd_already);
		tl = tl->next;
	}
	lvl3_remove_obsolete_tokens(toklst);
	split_tokens_with_whitespaces(toklst);
	return (0);
}

/**
 * Actually apply the lvl3 tokenization.
 *
 * At this point we might end up with multiple TOK_CMD-like tokens in one
 * pipe-section. Also there might processed quoted strings that are only
 * TOK_WORD or TOK_QWORD so far, empty quotes or not found variables. This is
 * all fixed and organized in here.
 */
static void	apply_lvl3_tokenization(t_token *cur, int *cmd_already)
{
	if (cur->type == TOK_PIP)
		*cmd_already = 0;
	else if ((cur->type == TOK_CMD || cur->type == TOK_WORD || \
				cur->type == TOK_QWORD) && *cmd_already)
		cur->type = TOK_ARG;
	else if (cur->type == TOK_CMD || cur->type == TOK_BLTIN)
		*cmd_already = 1;
	else if (cur->type == TOK_WORD && !*cmd_already)
	{
		if (cur->value[0] != 0)
		{
			cur->type = TOK_CMD;
			*cmd_already = 1;
		}
		else
			cur->type = TOK_NULL;
	}
	else if (cur->type == TOK_QWORD && !*cmd_already)
	{
		if (is_cmd_or_builtin(cur->value) == TOK_BLTIN)
			cur->type = TOK_BLTIN;
		else
			cur->type = TOK_QCMD;
		*cmd_already = 1;
	}
}

/* Check toklst before lvl3
 *
 * return(0) cases:
 * - f.ex. `ls 1>2>3`
 */
int	lvl3_check_toklst(t_toklst *toklst)
{
	t_token	*cur;
	t_token	*next;
	t_token	*prev;

	cur = toklst->token;
	prev = NULL;
	while (toklst->next)
	{
		next = toklst->next->token;
		if (!check_tok_lvl3(prev, cur, next))
			return (0);
		prev = cur;
		cur = next;
		toklst = toklst->next;
	}
	return (1);
}

int	check_tok_lvl3(t_token *prev, t_token *cur, t_token *next)
{
	(void)prev;
	if (cur->type == TOK_ROUT1 && next->type == TOK_ROUT_FDFROM)
		return (print_tokerr(TOKERR_FDFROM, next->value));
	return (1);
}

/**
 * Finally remove obsolete tokens at the end of lvl3.
 *
 * F.ex. if the only prompt-input was `$NOVAR` which does not exist, we would
 * end up with a TOK_CMD with empty string as its value. This should be removed.
 */
static void	lvl3_remove_obsolete_tokens(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_CMD && tl->token->value[0] == 0)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_NULL)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_HERE)
			toklst_remove_tok(toklst, &tl);
		if (tl)
			tl = tl->next;
	}
}

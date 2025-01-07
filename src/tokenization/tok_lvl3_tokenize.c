/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:24:38 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/07 12:30:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_toklst_lvl3(t_toklst *toklst);
static int	check_tok_lvl3(t_token *prev, t_token *cur, t_token *next);
static void	apply_lvl3_tokenization(t_token *cur, int *cmd_already);
void		lvl3_retok_white_cmds(t_toklst **tlst);

/**
 * Do the lvl3 tokenization.
 *
 * So far in lvl3 we have to fix some false tokenizations from lvl2 thje correct
 * classifaction of TOK_CMD and TOK_BLTIN tokens. So this function scans through
 * the whole toklst, finds the first TOK_CMD/BLTIN sets the cmd_already flag.
 * After that if there is another TOK_CMD/BLTIN before the next pipe this is a
 * false classification. This token then needs to beand gets turned into
 * TOK_ARG.
 */
int	tokenize_lvl3(t_toklst	**toklst)
{
	t_token		*cur;
	t_toklst	*tl;
	int			cmd_already;

	if (!*toklst)
		return (0);
	if (!check_toklst_lvl3(*toklst))
		return (0);
	tl = *toklst;
	cmd_already = 0;
	while (tl)
	{
		cur = tl->token;
		apply_lvl3_tokenization(cur, &cmd_already);
		tl = tl->next;
	}
	lvl3_retok_white_cmds(toklst);
	return (1);
}

/* Actually apply the lvl3 tokenization. */
static void	apply_lvl3_tokenization(t_token *cur, int *cmd_already)
{
	if (cur->type == TOK_PIP)
		*cmd_already = 0;
	else if ((cur->type == TOK_CMD || cur->type == TOK_WORD) && *cmd_already)
		cur->type = TOK_ARG;
	else if (cur->type == TOK_CMD || cur->type == TOK_BLTIN)
		*cmd_already = 1;
	else if (cur->type == TOK_WORD && !*cmd_already)
	{
		cur->type = TOK_CMD;
		*cmd_already = 1;
	}
}

/* Check toklst before lvl3
 *
 * return(0) cases:
 * - f.ex. `ls 1>2>3`
 */
int	check_toklst_lvl3(t_toklst *toklst)
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

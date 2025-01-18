/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_check_toklst.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:45:36 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 23:21:39 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			is_special_tok(t_toktype tok);
int			check_rout3(t_token *prev, t_token *cur, t_token *next);
int			show_tokerr_lvl2(t_toktype tok);
int			lvl2_do_check(t_token *prev, t_token *cur, t_token *next);
static int	lvl2_check_only_one_tok(t_toktype tok);

/**
 * Check toklst at beginning of lvl2.
 *
 * First of all: toklst cannot be NULL, bc this is checked in tok_tokenize
 * before we get called. Then, check if there is only one token. Error out if it
 * some lonely redirector or stuff like that.
 */
int	lvl2_check_toklst(t_toklst *toklst)
{
	t_token	*cur;
	t_token	*next;
	t_token	*prev;
	int		check;

	if (!toklst)
		return (1);
	cur = toklst->token;
	if (toklst->next == NULL && lvl2_check_only_one_tok(cur->type))
		return (1);
	prev = NULL;
	check = 0;
	while (toklst->next)
	{
		next = toklst->next->token;
		check = lvl2_do_check(prev, cur, next);
		if (check)
			return (check);
		prev = cur;
		cur = next;
		toklst = toklst->next;
	}
	if (is_special_tok(cur->type))
		return (print_tokerr(TOKERR_NL, NULL));
	return (0);
}

/* If there is only one tok, check it. */
int	lvl2_check_only_one_tok(t_toktype tok)
{
	if (tok == TOK_RIN0 || tok == TOK_ROUT0 || tok == TOK_ROUT1 || \
		tok == TOK_ROUT2 || tok == TOK_ROUT3 || tok == TOK_ROUTA0 || \
		tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || tok == TOK_RINOUT0)
		return (print_tokerr(TOKERR_NL, NULL));
	if (tok == TOK_PIP)
		return (print_tokerr(TOKERR_PIP, NULL));
	if (tok == TOK_HERE)
		return (print_tokerr(TOKERR_NL, NULL));
	return (0);
}

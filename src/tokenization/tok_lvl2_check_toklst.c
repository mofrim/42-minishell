/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_check_toklst.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:45:36 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/26 10:57:44 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_special_tok(t_toktype tok);
static int	check_rout3(t_token *prev, t_token *cur, t_token *next);
static int	show_first_tokerr_lvl2(t_toktype tok);
static int	show_tokerr_lvl2(t_toktype tok);

/* toklst cannot be NULL because this is checked in tokenize() beforehand. */
// FIXME: maybe refactor so show_first_tokerr_lvl2 is not needed
int	check_toklst_lvl2(t_toklst *toklst)
{
	t_token	*cur;
	t_token	*next;
	t_token	*prev;

	cur = toklst->token;
	if (toklst->next == NULL && !show_first_tokerr_lvl2(cur->type))
		return (0);
	prev = NULL;
	while (toklst->next)
	{
		next = toklst->next->token;
		if (prev && !check_rout3(prev, cur, next))
				return (print_tokerr(TOKERR_REDIR, next->value));
		if (is_special_tok(cur->type) && is_special_tok(next->type) && \
			!(cur->type == TOK_PIP && is_redir_tok(next->type)))
			return (show_tokerr_lvl2(next->type));
		prev = cur;
		cur = next;
		toklst = toklst->next;
	}
	if (is_special_tok(cur->type))
		return (print_tokerr(TOKERR_NL, NULL));
	return (1);
}

int	check_rout3(t_token *prev, t_token *cur, t_token *next)
{
	if (cur->type == TOK_ROUT3 && get_posint_numstr(prev->value) >= 0 && \
		get_posint_numstr(next->value) == -1)
		return (0);
	return (1);
}

int	is_special_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3 || tok == TOK_HERE || tok == TOK_RIN || \
		tok == TOK_PIP || tok == TOK_AND)
		return (1);
	return (0);
}

int	show_first_tokerr_lvl2(t_toktype tok)
{
	if (tok == TOK_RIN || tok == TOK_ROUT0 || tok == TOK_ROUT1 || \
		tok == TOK_ROUT2 || tok == TOK_ROUT3 || tok == TOK_ROUTA0 || \
		tok == TOK_ROUTA1 || tok == TOK_ROUTA2)
		return (print_tokerr(TOKERR_NL, NULL));
	if (tok == TOK_PIP)
		return (print_tokerr(TOKERR_PIP, NULL));
	if (tok == TOK_HERE)
		return (print_tokerr(TOKERR_NL, NULL));
	return (1);
}

int	show_tokerr_lvl2(t_toktype tok)
{
	if (tok == TOK_RIN)
		return (print_tokerr(TOKERR_RIN, NULL));
	if (tok == TOK_ROUT0)
		return (print_tokerr(TOKERR_ROUT, NULL));
	if (tok == TOK_ROUT1)
		return (print_tokerr(TOKERR_ROUT, NULL));
	if (tok == TOK_ROUT2)
		return (print_tokerr(TOKERR_ROUT2, NULL));
	if (tok == TOK_ROUT3)
		return (print_tokerr(TOKERR_ROUT3, NULL));
	if (tok == TOK_ROUTA0)
		return (print_tokerr(TOKERR_ROUTA, NULL));
	if (tok == TOK_ROUTA1)
		return (print_tokerr(TOKERR_ROUTA, NULL));
	if (tok == TOK_ROUTA2)
		return (print_tokerr(TOKERR_ROUTA2, NULL));
	if (tok == TOK_PIP)
		return (print_tokerr(TOKERR_PIP, NULL));
	if (tok == TOK_HERE)
		return (print_tokerr(TOKERR_HERE, NULL));
	if (tok == TOK_AND)
		return (print_tokerr(TOKERR_AND, NULL));
	return (0);
}

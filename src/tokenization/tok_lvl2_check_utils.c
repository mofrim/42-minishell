/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_check_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 23:05:46 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 23:20:48 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if tok is special. */
int	is_special_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3 || tok == TOK_HERE || tok == TOK_RIN0 || \
		tok == TOK_PIP || tok == TOK_AND)
		return (1);
	return (0);
}

/* For cases like `cmd 2>&bla` which is not okay >:( */
int	check_rout3(t_token *prev, t_token *cur, t_token *next)
{
	if (cur->type == TOK_ROUT3 && get_posint_numstr(prev->value) >= 0 && \
		get_posint_numstr(next->value) == -1)
		return (0);
	return (1);
}

/* Wrapper around print_tokerr() for easy printing by toktype. */
int	show_tokerr_lvl2(t_toktype tok)
{
	if (tok == TOK_RIN0)
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
	return (2);
}

/**
 * Do the actual toklst check in lvl2.
 *
 * Stuff like `| echo -n oui` is filtered out. Also the correct usage of
 * TOK_ROUT3 is checked and the usual non-sense like `> >` or ` < |` or whatnot.
 */
int	lvl2_do_check(t_token *prev, t_token *cur, t_token *next)
{
	if (!prev && cur->type == TOK_PIP)
		return (show_tokerr_lvl2(TOK_PIP));
	if (prev && !check_rout3(prev, cur, next))
		return (print_tokerr(TOKERR_REDIR, next->value));
	if (is_special_tok(cur->type) && is_special_tok(next->type) && \
			!(cur->type == TOK_PIP && is_redir_tok(next->type)))
		return (show_tokerr_lvl2(next->type));
	return (0);
}

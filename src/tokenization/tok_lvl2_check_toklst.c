/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_check_toklst.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:45:36 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 12:59:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_error_int(t_tokerr te)
{
	if (te == TOKERR_NL)
		ft_printf("Syntax error near unexpected token: \'\\n\'\n");
	if (te == TOKERR_PIP)
		ft_printf("Syntax error near unexpected token: \'|\'\n");
	if (te == TOKERR_RIN)
		ft_printf("Syntax error near unexpected token: \'<\'\n");
	if (te == TOKERR_ROUT)
		ft_printf("Syntax error near unexpected token: \'>\'\n");
	if (te == TOKERR_ROUTA)
		ft_printf("Syntax error near unexpected token: \'>>\'\n");
	if (te == TOKERR_HERE)
		ft_printf("Syntax error near unexpected token: \'<<\'\n");
	return (0);
}

static int	show_tokerr(t_toktype tok)
{
	if (tok == TOK_RIN)
		return (token_error_int(TOKERR_RIN));
	if (tok == TOK_ROUT)
		return (token_error_int(TOKERR_ROUT));
	if (tok == TOK_ROUTA)
		return (token_error_int(TOKERR_ROUTA));
	if (tok == TOK_PIP)
		return (token_error_int(TOKERR_PIP));
	if (tok == TOK_HERE)
		return (token_error_int(TOKERR_HERE));
	return (0);
}

static	int	show_first_tokerr(t_toktype tok)
{
	if (tok == TOK_RIN || tok == TOK_ROUT || \
			tok == TOK_ROUTA)
		return (token_error_int(TOKERR_NL));
	if (tok == TOK_PIP)
		return (token_error_int(TOKERR_PIP));
	if (tok == TOK_HERE)
		return (token_error_int(TOKERR_NL));
	return (1);
}

/* toklst cannot be NULL because this is checked in tokenize() beforehand. */
int	check_toklst_lvl2(t_toklst *toklst)
{
	t_toktype	cur;
	t_toktype	next;

	cur = toklst->token->type;
	if (toklst->next == NULL && !show_first_tokerr(cur))
		return (0);
	while (toklst->next)
	{
		next = toklst->next->token->type;
		if ((cur == TOK_RIN || cur == TOK_ROUT || cur == TOK_ROUTA || \
			cur == TOK_HERE) && (next == TOK_RIN || next == TOK_ROUT \
			|| next == TOK_ROUTA || next == TOK_PIP || next == TOK_HERE))
			return (show_tokerr(next));
		cur = next;
		toklst = toklst->next;
	}
	if (cur == TOK_RIN || cur == TOK_ROUT || cur == TOK_ROUTA || \
		cur == TOK_HERE)
		return (token_error_int(TOKERR_NL));
	return (1);
}

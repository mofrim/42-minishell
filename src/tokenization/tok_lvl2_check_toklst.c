/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_check_toklst.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 18:45:36 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 20:40:11 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	show_first_tokerr(t_toktype tok);
static int	token_error_int(t_tokerr te);
static int	show_tokerr(t_toktype tok);
static int	is_special_tok(t_toktype tok);

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
		if (is_special_tok(cur) && is_special_tok(next))
			return (show_tokerr(next));
		cur = next;
		toklst = toklst->next;
	}
	if (is_special_tok(cur))
		return (token_error_int(TOKERR_NL));
	return (1);
}

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
	if (te == TOKERR_ROUT2)
		ft_printf("Syntax error near unexpected token: \'&>\'\n");
	if (te == TOKERR_ROUT3)
		ft_printf("Syntax error near unexpected token: \'>&\'\n");
	if (te == TOKERR_ROUTA)
		ft_printf("Syntax error near unexpected token: \'>>\'\n");
	if (te == TOKERR_ROUTA2)
		ft_printf("Syntax error near unexpected token: \'&>>\'\n");
	if (te == TOKERR_HERE)
		ft_printf("Syntax error near unexpected token: \'<<\'\n");
	if (te == TOKERR_AND)
		ft_printf("Syntax error near unexpected token: \'&\'\n");
	return (0);
}

int	show_tokerr(t_toktype tok)
{
	if (tok == TOK_RIN)
		return (token_error_int(TOKERR_RIN));
	if (tok == TOK_ROUT0)
		return (token_error_int(TOKERR_ROUT));
	if (tok == TOK_ROUT1)
		return (token_error_int(TOKERR_ROUT));
	if (tok == TOK_ROUT2)
		return (token_error_int(TOKERR_ROUT2));
	if (tok == TOK_ROUT3)
		return (token_error_int(TOKERR_ROUT3));
	if (tok == TOK_ROUTA0)
		return (token_error_int(TOKERR_ROUTA));
	if (tok == TOK_ROUTA1)
		return (token_error_int(TOKERR_ROUTA));
	if (tok == TOK_ROUTA2)
		return (token_error_int(TOKERR_ROUTA2));
	if (tok == TOK_PIP)
		return (token_error_int(TOKERR_PIP));
	if (tok == TOK_HERE)
		return (token_error_int(TOKERR_HERE));
	if (tok == TOK_AND)
		return (token_error_int(TOKERR_AND));
	return (0);
}

int	show_first_tokerr(t_toktype tok)
{
	if (tok == TOK_RIN || tok == TOK_ROUT0 || tok == TOK_ROUT1 || \
		tok == TOK_ROUT2 || tok == TOK_ROUT3 || tok == TOK_ROUTA0 || \
		tok == TOK_ROUTA1 || tok == TOK_ROUTA2)
		return (token_error_int(TOKERR_NL));
	if (tok == TOK_PIP)
		return (token_error_int(TOKERR_PIP));
	if (tok == TOK_HERE)
		return (token_error_int(TOKERR_NL));
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

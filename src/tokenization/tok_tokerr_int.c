/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokerr_int.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 17:26:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/25 17:36:03 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_error_int_part1(t_tokerr te, char *tok);
static void	token_error_int_part2(t_tokerr te, char *tok);

int	token_error_int(t_tokerr te, char *tok)
{
	token_error_int_part1(te, tok);
	token_error_int_part2(te, tok);
	return (0);
}

void	token_error_int_part1(t_tokerr te, char *tok)
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
	if (te == TOKERR_REDIR)
		ft_printf("minishell: %s: ambiguous redirect\n", tok);
}

void	token_error_int_part2(t_tokerr te, char *tok)
{
	if (te == TOKERR_FDFROM)
		ft_dprintf(2, "minishell: Syntax error near unexpected token:"\
				" \'\%s\'\n", tok);
}

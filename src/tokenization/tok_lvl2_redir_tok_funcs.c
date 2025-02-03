/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_redir_tok_funcs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:48:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:48:54 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_routa1(t_token *prev, t_token *cur, t_token *next)
{
	(void)cur;
	if (get_posint_numstr(prev->value) >= 0 && \
			get_posint_numstr(next->value) >= 0)
	{
		prev->type = TOK_ROUTA_FDFROM;
		next->type = TOK_ROUTA_FDTO;
	}
	else if (get_posint_numstr(prev->value) >= 0 && \
			get_posint_numstr(next->value) == -1)
	{
		prev->type = TOK_ROUTA_FDFROM;
		next->type = TOK_OF;
	}
	else
		next->type = TOK_OF;
}

void	tokenize_routa2(t_token *prev, t_token *cur, t_token *next)
{
	(void)prev;
	(void)cur;
	next->type = TOK_OF;
}

void	tokenize_rin1(t_token *prev, t_token *cur, t_token *next)
{
	if (get_posint_numstr(prev->value) >= 0 && (next->type == TOK_WORD || \
				next->type == TOK_IF))
		prev->type = TOK_RIN_FDTO;
	else
	{
		cur->type = TOK_RIN0;
		prev->type = TOK_ARG;
	}
	if (is_word_tok(next->type))
		next->type = TOK_IF;
}

void	tokenize_rinout(t_token *prev, t_token *cur, t_token *next)
{
	if (cur->type == TOK_RINOUT1 && get_posint_numstr(prev->value) >= 0)
		prev->type = TOK_IFOF_FD;
	else if (cur->type == TOK_RINOUT1)
	{
		prev->type = TOK_ARG;
		cur->type = TOK_RINOUT0;
	}
	if (is_word_tok(next->type) || next->type == TOK_ARG)
		next->type = TOK_IFOF;
}

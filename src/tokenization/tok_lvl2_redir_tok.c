/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_redir_tok.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 06:39:12 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:49:55 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Funcs only used in this file. */
static void	tokenize_rout1(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_rout2(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_rout3(t_token *prev, t_token *cur, t_token *next);
void		tokenize_routa1(t_token *prev, t_token *cur, t_token *next);
void		tokenize_routa2(t_token *prev, t_token *cur, t_token *next);
void		tokenize_rin1(t_token *prev, t_token *cur, t_token *next);
void		tokenize_rinout(t_token *prev, t_token *cur, t_token *next);

void	apply_redir_tokenization(t_token *prev, t_token *cur, \
		t_token *next)
{
	if (prev)
	{
		if (cur->type == TOK_ROUT1)
			tokenize_rout1(prev, cur, next);
		if (cur->type == TOK_ROUT2)
			tokenize_rout2(prev, cur, next);
		if (cur->type == TOK_ROUT3)
			tokenize_rout3(prev, cur, next);
		if (cur->type == TOK_ROUTA1)
			tokenize_routa1(prev, cur, next);
		if (cur->type == TOK_ROUTA2)
			tokenize_routa2(prev, cur, next);
		if (cur->type == TOK_RIN1)
			tokenize_rin1(prev, cur, next);
		if (cur->type == TOK_RINOUT0 || cur->type == TOK_RINOUT1)
			tokenize_rinout(prev, cur, next);
	}
}

/* Figure out if we really got TOK_ROUT1, which means something of the form
 * n>word where n is some positive int. Set the toktypes accordingly. */
static void	tokenize_rout1(t_token *prev, t_token *cur, t_token *next)
{
	(void)cur;
	if (get_posint_numstr(prev->value) >= 0 && (is_word_tok(next->type) || \
				next->type == TOK_OF))
		prev->type = TOK_ROUT_FDFROM;
	else
	{
		cur->type = TOK_ROUT0;
		prev->type = TOK_ARG;
	}
	if (is_word_tok(next->type))
		next->type = TOK_OF;
}

/* deal with '&>' token.  */
static void	tokenize_rout2(t_token *prev, t_token *cur, t_token *next)
{
	(void)cur;
	if (is_word_tok(prev->type))
		prev->type = TOK_ARG;
	next->type = TOK_OF;
}

/* deal with '>&' token. */
static void	tokenize_rout3(t_token *prev, t_token *cur, t_token *next)
{
	(void)cur;
	if (get_posint_numstr(prev->value) >= 0 && \
			get_posint_numstr(next->value) >= 0)
	{
		prev->type = TOK_ROUT3_FDFROM;
		next->type = TOK_ROUT3_FDTO;
	}
	else if (get_posint_numstr(prev->value) == -1)
	{
		prev->type = TOK_ARG;
		next->type = TOK_ROUT3_FDTO;
	}
}

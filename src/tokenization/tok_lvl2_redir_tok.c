/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_redir_tok.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 06:39:12 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/12 15:16:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	tokenize_rout1(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_rout2(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_rout3(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_routa2(t_token *prev, t_token *cur, t_token *next);
static void	tokenize_routa1(t_token *prev, t_token *cur, t_token *next);

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
			tokenize_routa2(prev, cur, next);
		if (cur->type == TOK_ROUTA2)
			tokenize_routa2(prev, cur, next);
	}
}

/* Determine wether a string represents a positive int number. Return its value
 * if true, -1 otherwise. */
static int	is_posint_numstr(char *s)
{
	long	num;

	num = 0;
	while (ft_isdigit(*s))
	{
		num = num * 10 + *s++ - '0';
		if (num > INT_MAX)
			return (-1);
	}
	if (!ft_isdigit(*s) && *s)
		return (-1);
	return (num);
}

static void	tokenize_rout1(t_token *prev, t_token *cur, t_token *next)
{
	if (is_posint_numstr(prev->value) && next->type == TOK_WORD)
		prev->type = TOK_ROUT_FILDES_IN;
	else
		prev->type = TOK_ARG;
	if (next->type == TOK_WORD)
		next->type = TOK_OF;
}

/* deal with '&>' token.  */
static void	tokenize_rout2(t_token *prev, t_token *cur, t_token *next)
{
		prev->type = TOK_ARG;
		next->type = TOK_OF;
}

/* deal with '>&' token. */
static void	tokenize_rout3(t_token *prev, t_token *cur, t_token *next)
{
	if (is_posint_numstr(prev->value) >= 0 && \
			is_posint_numstr(next->value) >= 0)
	{
		prev->type = TOK_ROUT_FILDES_IN;
		next->type = TOK_ROUT3_FILDES_OUT;
	}
	else if (is_posint_numstr(prev->value) == -1)
	{
		prev->type = TOK_ARG;
		next->type = TOK_ROUT3_FILDES_OUT;
	}
}

static void	tokenize_routa1(t_token *prev, t_token *cur, t_token *next)
{
	if (is_posint_numstr(prev->value) && next->type == TOK_WORD)
	{
		prev->type = TOK_ROUT_FILDES_IN;
		next->type = TOK_OF;
	}
	else
	{
		prev->type = TOK_ARG;
		next->type = TOK_OF;
	}
}

static void	tokenize_routa2(t_token *prev, t_token *cur, t_token *next)
{
	if (is_posint_numstr(prev->value) >= 0 && \
			is_posint_numstr(next->value) >= 0)
	{
		prev->type = TOK_ROUT_FILDES_IN;
		next->type = TOK_ROUT3_FILDES_OUT;
	}
	else if (is_posint_numstr(prev->value) == -1)
	{
		prev->type = TOK_ARG;
		next->type = TOK_ROUT3_FILDES_OUT;
	}
}

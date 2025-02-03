/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 00:21:29 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:43:47 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Is t_toktype arg TOK_QWORD or TOK_WORD? */
int	is_word_tok(t_toktype tt)
{
	if (tt == TOK_WORD || tt == TOK_QWORD || tt == TOK_VWORD)
		return (1);
	return (0);
}

/* Is current toktype a output redirection token? */
int	is_rout_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3 || tok == TOK_RINOUT0 || tok == TOK_RINOUT1)
		return (1);
	return (0);
}

int	is_rin_tok(t_toktype tok)
{
	if (tok == TOK_RIN0 || tok == TOK_RIN1 || tok == TOK_RINOUT0 || \
		tok == TOK_RINOUT1 || tok == TOK_HERE)
		return (1);
	return (0);
}

/* Also include TOK_RIN here. */
int	is_redir_tok(t_toktype tok)
{
	if (is_rout_tok(tok) || is_rin_tok(tok))
		return (1);
	return (0);
}

/* Return 1 if we have a whitspace char in string str. Else 0. */
int	has_whitespace(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

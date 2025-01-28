/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 00:21:29 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 22:57:27 by fmaurer          ###   ########.fr       */
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


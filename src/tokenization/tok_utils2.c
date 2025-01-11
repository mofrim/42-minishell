/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 00:21:29 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/12 00:25:36 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Is t_toktype arg TOK_QWORD or TOK_WORD? */
int	is_word_tok(t_toktype tt)
{
	if (tt == TOK_WORD || tt == TOK_QWORD)
		return (1);
	return (0);
}

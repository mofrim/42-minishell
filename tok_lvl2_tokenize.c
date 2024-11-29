/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:57:29 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/28 20:21:42 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/* Tokenization Level 2. Goal is to classify all the TOK_WORD tokens and rule
 * out some invalid syntax like
 *
 * 	- "... < < ... "
 * 	- "... | <"
 * 	- "<< |"
 * 	- "<< <"
 * 	- "<< >"
 * 	- a lot more
 *
 * 	Rulez:
 * 	1) after >, <, >>, and << there HAS to be a TOK_WORD!
 * 	2) VAR_NAMES cannot contain |, <, >, <<, >>, ", '
 * 	3) | cannot be the first token
 */
// TODO: add seperate validator func that checks tokenlist for invalid sequences
// in advance, maybe?!
// TODO: token_error(): prints "unexpected token ..." and makes this function
// return NULL
int	tokenize_lvl2(t_tokenlist	*toklst)
{
	t_toktype	last;
	t_toktype	cur;
	t_toktype	next;

	if (!check_toklst_lvl2(toklst))
		return (0);
	cur = toklst->token->type;
	if (cur == TOK_WORD)
		cur = TOK_CMD;
	while (toklst->next)
	{
		next = toklst->next->token->type;
		if (cur == TOK_RIN && next == TOK_WORD)
			next = TOK_IF;
		if (cur == TOK_ROUT && next == TOK_WORD)
			next = TOK_OF;
		if ((cur == TOK_IF || cur == TOK_OF || \
					cur == TOK_PIP) && next == TOK_WORD)
			next = TOK_CMD;
		if (cur == TOK_CMD && next == TOK_WORD)
			next = TOK_ARG;
		cur = next;
		toklst = toklst->next;
	}
	return (1);
}

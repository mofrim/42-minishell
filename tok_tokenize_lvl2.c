/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize_lvl2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:57:29 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/28 11:14:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*token_error(t_token *tok)
{
	ft_printf("Unexpected token: \"%s\"\n", tok->value);
	return (NULL);
}

/* toklst cannot be NULL because this is checkend in tokenize() beforehand. */
int	check_toklst_lvl2(t_tokenlist *toklst)
{
	t_token *cur;
	t_token *next;

	cur = toklst->token;
	if (toklst->next == NULL)
		if (cur->type == TOK_PIP || cur->type == TOK_RIN || \
				cur->type == TOK_ROUT || cur->type == TOK_ROUTA)
	while (toklst->next)

}

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
t_tokenlist	*tokenize_lvl2(t_tokenlist	*toklst)
{
	t_token		*last;
	t_token		*cur;
	t_token		*next;
	t_tokenlist *orig_lst;

	orig_lst = toklst;
	last = NULL;
	cur = toklst->token;
	if (cur->type == TOK_WORD)
		cur->type = TOK_CMD;
	while (toklst->next)
	{
		next = toklst->next->token;
		if (cur->type == TOK_RIN && next->type == TOK_WORD)
			next->type = TOK_IF;
		if (cur->type == TOK_ROUT && next->type == TOK_WORD)
			next->type = TOK_OF;
		if ((cur->type == TOK_IF || cur->type == TOK_OF || \
					cur->type == TOK_PIP) && next->type == TOK_WORD)
			next->type = TOK_CMD;
		if (cur->type == TOK_CMD && next->type == TOK_WORD)
			next->type = TOK_ARG;
		cur = next;
		toklst = toklst->next;
	}
	return (orig_lst);
}

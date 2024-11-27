/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/27 23:25:37 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * TODO: rewrite tokenization in a satisfying way.
 */

/* Initialize tokenizer with input string. Skip any whitespace chars at the
 * begin. */
t_cmdline	*init_cmdline(char *input)
{
	t_cmdline	*cl;

	cl = malloc(sizeof(t_cmdline));
	nullcheck(cl, "init_cmdline");
	cl->input = input;
	cl->pos = 0;
	cl->length = ft_strlen(input);
	cl->dquot_flag = 0;
	cl->squot_flag = 0;
	cl->var_flag = 0;
	return (cl);
}

/* Tokenization Level 1. No error checking here. Only as little as possible
 * grammar applied (mostly quotation related).
 */
t_tokenlist	*tokenize_lvl1(char *input)
{
	t_cmdline		*cmdline;
	t_tokenlist		*tok_lst;
	t_tokenlist		*tmplst;
	t_token			*tmptok;

	cmdline = init_cmdline(input);
	tmptok = get_next_token(cmdline);
	if (tmptok == NULL)
		return (NULL);
	tok_lst = toklst_new(tmptok);
	nullcheck(tok_lst, "toklst_new");
	while (tmptok)
	{
		tmptok = get_next_token(cmdline);
		if (tmptok)
		{
			tmplst = toklst_new(tmptok);
			nullcheck(tok_lst, "toklst_new");
			toklst_add_back(&tok_lst, tmplst);
		}
	}
	return (tok_lst);
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

t_tokenlist	*tokenize(char *input)
{
	t_tokenlist	*toklst_lvl1;
	t_tokenlist	*toklst_lvl2;

	toklst_lvl1 = tokenize_lvl1(input);
	if (toklst_lvl1 == NULL)
		return (NULL);
	toklst_lvl2 = tokenize_lvl2(toklst_lvl1);
	return (toklst_lvl2);
}

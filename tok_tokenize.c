/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/24 22:42:56 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * TODO: rewrite tokenization in a satisfying way.
 */

/* Initialize tokenizer with input string. Skip any whitespace chars at the
 * begin. */
t_cmdline	*init_cmdline(char* input) 
{
    t_cmdline	*cl = malloc(sizeof(t_cmdline)); // FIXME handle malloc
    cl->input = input;
    cl->pos = 0;
    cl->length = ft_strlen(input);
	cl->dquot_flag = 0;
	cl->squot_flag = 0;
	cl->var_flag = 0;
    return (cl);
}

t_tokenlist	*tokenize(char *input)
{
	t_cmdline	*cmdline;
	t_tokenlist		*tok_lst;
	t_token			*tmptok;

	cmdline = init_cmdline(input);
	tmptok = get_next_token(cmdline);
	if (tmptok == NULL)
		return (NULL);
	tok_lst = toklst_new(tmptok); // FIXME handle malloc
	while (tmptok)
	{
		tmptok = get_next_token(cmdline);
		if (tmptok)
			toklst_add_back(&tok_lst, toklst_new(tmptok));
	}
	return (tok_lst);
}

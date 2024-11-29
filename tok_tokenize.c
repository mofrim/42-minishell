/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/29 08:32:45 by fmaurer          ###   ########.fr       */
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

t_tokenlist	*tokenize(char *input)
{
	t_tokenlist	*toklst;

	toklst = tokenize_lvl1(input);
	if (!toklst)
		return (NULL);
	if (!tokenize_lvl2(toklst))
		return (NULL);
	return (toklst);
}

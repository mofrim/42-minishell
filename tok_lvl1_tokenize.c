/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 08:32:18 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/29 08:32:42 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 08:32:18 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 11:05:13 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Tokenization Level 1.
 *
 * Almost no syntactical error checking here. Only as little as possible grammar
 * applied. Most work is done inside the get_next_token func. Envvars are
 * already resolved in here for further processing their content in
 * `get_tok_var_value`. Also most possible redir tokens are classified.
 *
 * The only way this level can return error, is when malloc fails somewhere bc
 * if the cmdline was empty in the beginning we would not have entered
 * tokenization at all.
 */
t_toklst	*tokenize_lvl1(char *input, t_envlst *env)
{
	t_cmdline		*cmdline;
	t_toklst		*tok_lst;
	t_toklst		*tmplst;
	t_token			*tmptok;

	cmdline = init_cmdline(input, env);
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
	free(cmdline);
	return (tok_lst);
}

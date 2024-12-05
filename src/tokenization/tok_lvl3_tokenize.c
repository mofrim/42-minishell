/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:24:00 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 15:43:18 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_tok(t_toklst **toklst, t_toklst **tl)
{
	t_toklst	*tmp;

	tmp = (*tl)->next;
	toklst_del(toklst, *tl);
	*tl = tmp;
}

static void	remove_quot_varsym(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_VAR_SYM)
			remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_DQUOT)
			remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_SQUOT)
			remove_tok(toklst, &tl);
		else
			tl = tl->next;
	}
}

// Uffffff! I really start to think that this substitution should have happened
// way earlier!!! 🤯
static void	substitute_envvar(t_toklst *tlst, t_envlst *env)
{
}

void	tokenize_lvl3(t_toklst **toklst)
{
	remove_quot_varsym(toklst);
}

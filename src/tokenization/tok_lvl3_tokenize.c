/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 13:24:00 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 13:34:22 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_lvl3(t_toklst **toklst)
{
	t_toklst	*tl;
	t_toklst	*tmp;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_VAR_SYM)
		{
			tmp = tl->next;
			toklst_del(toklst, tl);
			tl = tmp;
		}
		else if (tl->token->type == TOK_DQUOT)
		{
			tmp = tl->next;
			toklst_del(toklst, tl);
			tl = tmp;
		}
		else if (tl->token->type == TOK_SQUOT)
		{
			tmp = tl->next;
			toklst_del(toklst, tl);
			tl = tmp;
		}
		else
			tl = tl->next;
	}
}

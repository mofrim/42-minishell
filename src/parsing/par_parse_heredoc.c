/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:06:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/14 22:02:58 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_heredoc(t_toklst **tlst, t_cmdlst *cur_cmd)
{
	t_toklst	*tl;
	t_redirlst	rfl;

	tl = *tlst;
	if (tl && (tl->token->type == TOK_HERE_DLIM || \
				tl->token->type == TOK_HERE_QDLIM))
	{
		herdlst_add_back(&cur_cmd->heredocs, herdlst_new(tl->token));
		init_redirlst_var(&rfl);
		if (tl->token->type == TOK_HERE_DLIM)
			rfl.redtype = RE_DOC;
		else
			rfl.redtype = RE_QDOC;
		rfl.infile = tl->token->value;
		redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
		*tlst = (*tlst)->next;
	}
}

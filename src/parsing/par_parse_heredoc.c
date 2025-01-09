/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:06:22 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/13 16:10:38 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_heredoc(t_toklst **tlst, t_cmdlst *cur_cmd)
{
	t_toklst	*tl;
	t_redirlst	rfl;

	tl = *tlst;
	if (tl && tl->token->type == TOK_HERE_DLIM)
	{
		herdlst_add_back(&cur_cmd->heredocs, herdlst_new(tl->token->value));
		init_redirlst_var(&rfl);
		rfl.redtype = RE_HERE;
		rfl.infile = tl->token->value;
		redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
		*tlst = (*tlst)->next;
	}
}

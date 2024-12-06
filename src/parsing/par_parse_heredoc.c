/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:06:22 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 15:43:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		parse_heredoc(t_toklst **tlst, t_cmdlst *cur_cmd)
{
	t_toklst *tl;

	tl = *tlst;
	if (tl && tl->token->type == TOK_HERE_DLIM)
	{
		cur_cmd->heredoc = ft_strdup(tl->token->value);
		*tlst = (*tlst)->next;
	}
}

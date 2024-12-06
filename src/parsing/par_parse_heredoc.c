/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:06:22 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/06 15:29:02 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Level 3. Handle all heredocs. 
 *
 * 1) Scan through the toklst and search for valid (?!) heredoc pairs. while
 * doing this remove the TOK_HEREs.
 * 2) while scanning also collect the delimiters bc we will need them for
 * correct termination behavior of the get_heredoc_input() -> termination only
 * by entering the delims FIFO.
 *
 * */
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

// t_toklst	*tl;
//
// tl = *tlst;
// while (tl)
// {
// 	if(tl->token->type == TOK_HERE)
// 		toklst_remove_tok(tlst, &tl);
// 	else if (tl->token->type == TOK_HERE_DLIM)
//
// 	else
// 		tl = tl->next;
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:24:38 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 20:28:44 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	apply_lvl3_tokenization(t_token *prev, t_token *cur, \
		t_token *next, int *cmd_found);

int	tokenize_lvl3(t_toklst	**toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;
	int			cmd_found;

	if (!*toklst)
		return (0);
	tl = *toklst;
	cur = tl->token;
	prev = NULL;
	cmd_found = 0;
	while (tl->next)
	{
		next = tl->next->token;
		if (cur->type == TOK_CMD || cur->type == TOK_BLTIN)
			cmd_found = 1;
		if (prev)
			apply_lvl3_tokenization(prev, cur, next, &cmd_found);
		prev = cur;
		cur = next;
		tl = tl->next;
	}
	apply_lvl3_tokenization(prev, cur, next, &cmd_found);
	return (1);
}

static void	apply_lvl3_tokenization(t_token *prev, t_token *cur, \
		t_token *next, int *cmd_found)
{
	if (cur->type == TOK_PIP)
		*cmd_found = 0;
	if ((cur->type == TOK_CMD || cur->type == TOK_WORD) && *cmd_found)
		cur->type = TOK_ARG;
}

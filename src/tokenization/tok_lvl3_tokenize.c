/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 15:24:38 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/26 10:56:21 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	void	apply_lvl3_tokenization(t_token *prev, t_token *cur, \
										t_token *next, int *cmd_found);
static	int		check_toklst_lvl3(t_toklst *toklst);
static	int		check_tok_lvl3(t_token *prev, t_token *cur, t_token *next);

int	tokenize_lvl3(t_toklst	**toklst)
{
	t_token		*cur;
	t_token		*next;
	t_token		*prev;
	t_toklst	*tl;
	int			cmd_already;

	if (!*toklst)
		return (0);
	if (!check_toklst_lvl3(*toklst))
		return (0);
	tl = *toklst;
	cur = tl->token;
	prev = NULL;
	cmd_already = 0;
	while (tl->next)
	{
		next = tl->next->token;
		apply_lvl3_tokenization(prev, cur, next, &cmd_already);
		prev = cur;
		cur = next;
		tl = tl->next;
	}
	apply_lvl3_tokenization(prev, cur, next, &cmd_already);
	return (1);
}

static void	apply_lvl3_tokenization(t_token *prev, t_token *cur, \
		t_token *next, int *cmd_already)
{
	if (cur->type == TOK_PIP)
		*cmd_already = 0;
	else if ((cur->type == TOK_CMD || cur->type == TOK_WORD) && *cmd_already)
		cur->type = TOK_ARG;
	else if (cur->type == TOK_CMD || cur->type == TOK_BLTIN)
		*cmd_already = 1;
}

/* Check toklst before lvl3. return(0) cases:
 * - `ls 1>2>3`*/
int	check_toklst_lvl3(t_toklst*toklst)
{
	t_token	*cur;
	t_token	*next;
	t_token	*prev;

	cur = toklst->token;
	prev = NULL;
	while (toklst->next)
	{
		next = toklst->next->token;
		if (!check_tok_lvl3(prev, cur, next))
			return (0);
		prev = cur;
		cur = next;
		toklst = toklst->next;
	}
	return (1);
}

int	check_tok_lvl3(t_token *prev, t_token *cur, t_token *next)
{
	if (cur->type == TOK_ROUT1 && next->type == TOK_ROUT_FDFROM)
		return (print_tokerr(TOKERR_FDFROM, next->value));
	return (1);
}

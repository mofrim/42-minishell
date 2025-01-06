/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl3_retok_white_cmds.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 12:22:58 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 14:49:56 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_whitespace(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

t_toklst	*split_cmd(char *cmdstr)
{
	t_toklst	*tlst;
	t_token		*new;
	char		**cmdstr_split;
	int			i;
	int			splsize;

	tlst = NULL;
	cmdstr_split = ft_split(cmdstr, ' ');
	i = 0;
	splsize = splitsize(cmdstr_split);
	new = init_token(cmdstr_split[i], is_cmd_or_builtin(cmdstr_split[i]));
	toklst_add_back(&tlst, toklst_new(new));
	while (++i < splsize)
	{
		new = init_token(cmdstr_split[i], TOK_ARG);
		toklst_add_back(&tlst, toklst_new(new));
	}
	free_ptrptr(&cmdstr_split);
	return (tlst);
}

void	free_tok(t_token *tok)
{
	free(tok->value);
	free(tok);
}

/* Insert the split cmd with its args into the toklst at the same position where
 * it was before. */
void	insert_splitcmd_in_toklst(t_toklst *splitcmd_toklst, t_toklst **tl,
									t_toklst **tlst)
{
	if (toklst_size(*tlst) == 1)
	{
		*tlst = splitcmd_toklst;
		free_tok((*tl)->token);
		*tl = NULL;
		return ;
	}
	ft_printf(RED "<< DEBUG >> (*tl)->prev->value = %p\n" RST, (*tl)->prev->token->value);
	if ((*tl)->prev)
		(*tl)->prev->next = splitcmd_toklst;
	if ((*tl)->next)
		(*tl)->next->prev = toklst_last(splitcmd_toklst);
	free_tok((*tl)->token);
	*tl = (*tl)->next;
}

void		lvl3_retok_white_cmds(t_toklst **tlst)
{
	t_toklst	*tl;
	t_token		*tok;
	t_toklst	*splitcmd_toklst;

	tl = *tlst;
	while (tl)
	{
		tok = tl->token;
		if ((tok->type == TOK_CMD || tok->type == TOK_BLTIN) && \
				has_whitespace(tok->value))
		{
			ft_printf(RED "<< DEBUG >> tok->value in retok = %s\n" RST, tok->value);
			splitcmd_toklst = split_cmd(tok->value);
			insert_splitcmd_in_toklst(splitcmd_toklst, &tl, tlst);
		}
		else
			tl = tl->next;
	}
}

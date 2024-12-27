/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:10:47 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 15:21:17 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* If given token is a builtin return TOK_BUILTIN, else if token represents a
 * positive number -> TOK_CMD, else -> TOK_WORD. */
t_toktype	is_cmd_or_builtin(t_token *tok)
{
	char	*val;

	val = tok->value;
	if (!ft_strcmp(val, "echo") || !ft_strcmp(val, "cd") || \
			!ft_strcmp(val, "pwd") || !ft_strcmp(val, "export") || \
			!ft_strcmp(val, "unset") || !ft_strcmp(val, "env") || \
			!ft_strcmp(val, "exit"))
		return (TOK_BLTIN);
	else if (get_posint_numstr(tok->value) == -1)
		return (TOK_CMD);
	return (TOK_WORD);
}

/* Is current toktype a output redirection token? */
int	is_rout_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3)
		return (1);
	return (0);
}

/* Also include TOK_RIN here. */
int	is_redir_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3 || tok == TOK_RIN0)
		return (1);
	return (0);
}

/* Subroutine for deleting exactly one token from a toklst. */
void	toklst_remove_tok(t_toklst **toklst, t_toklst **tl)
{
	t_toklst	*tmp;

	tmp = (*tl)->next;
	toklst_del(toklst, *tl);
	*tl = tmp;
}

/* Remove some obsolete tokens in lvl2. */
void	remove_obsolete_tokens(t_toklst **toklst)
{
	t_toklst	*tl;

	tl = *toklst;
	while (tl)
	{
		if (tl->token->type == TOK_VAR_SYM)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_DQUOT)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_SQUOT)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_AND)
			toklst_remove_tok(toklst, &tl);
		else if (tl->token->type == TOK_WORD && tl->token->value[0] == 0)
			toklst_remove_tok(toklst, &tl);
		else
			tl = tl->next;
	}
}

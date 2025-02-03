/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:10:47 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:46:14 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* If given token is a builtin return TOK_BUILTIN, else if token represents a
 * positive number, if not -> TOK_CMD, else -> TOK_WORD. Meaning: if we have a
 * number this might be a fd number for some redir. */
t_toktype	is_cmd_or_builtin(char *val)
{
	if (!ft_strcmp(val, "echo") || !ft_strcmp(val, "cd") || \
			!ft_strcmp(val, "pwd") || !ft_strcmp(val, "export") || \
			!ft_strcmp(val, "unset") || !ft_strcmp(val, "env") || \
			!ft_strcmp(val, "exit"))
		return (TOK_BLTIN);
	else if (get_posint_numstr(val) == -1)
		return (TOK_CMD);
	return (TOK_WORD);
}

/* Subroutine for deleting exactly one token from a toklst. */
void	toklst_remove_tok(t_toklst **toklst, t_toklst **tl)
{
	t_toklst	*tmp;

	tmp = (*tl)->next;
	toklst_del(toklst, *tl);
	*tl = tmp;
}

/* Init a token with value and type. */
t_token	*init_token(char *value, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	nullcheck(tok, "init_token()");
	if (!tok)
		return (NULL);
	tok->type = type;
	if (value)
		tok->value = ft_strdup(value);
	else
		tok->value = NULL;
	return (tok);
}

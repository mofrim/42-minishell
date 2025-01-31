/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:10:47 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/31 12:00:59 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* If given token is a builtin return TOK_BUILTIN, else if token represents a
 * positive number, if not -> TOK_CMD, else -> TOK_WORD. */
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

/* Is current toktype a output redirection token? */
int	is_rout_tok(t_toktype tok)
{
	if (tok == TOK_ROUTA0 || tok == TOK_ROUTA1 || tok == TOK_ROUTA2 || \
		tok == TOK_ROUT0 || tok == TOK_ROUT1 || tok == TOK_ROUT2 || \
		tok == TOK_ROUT3 || tok == TOK_RINOUT0 || tok == TOK_RINOUT1)
		return (1);
	return (0);
}

int	is_rin_tok(t_toktype tok)
{
	if (tok == TOK_RIN0 || tok == TOK_RIN1 || tok == TOK_RINOUT0 || \
		tok == TOK_RINOUT1 || tok == TOK_HERE)
		return (1);
	return (0);
}

/* Also include TOK_RIN here. */
int	is_redir_tok(t_toktype tok)
{
	if (is_rout_tok(tok) || is_rin_tok(tok))
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

t_token	*init_token(char *value, int type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	if (value)
		tok->value = ft_strdup(value);
	else
		tok->value = NULL;
	return (tok);
}

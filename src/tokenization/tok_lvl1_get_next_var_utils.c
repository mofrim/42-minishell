/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_var_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 08:47:18 by elpah             #+#    #+#             */
/*   Updated: 2025/02/03 10:53:54 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check wether string after a '$' is a valid variable name.
 * This is the case if it is
 * - alphanumeric or
 * - underscore or
 * - or a special varname like $? (.., $$, $*, $@). in this case return 2, not
 *   1! */
int	is_valid_varname_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	if (c == '?')
		return (2);
	return (0);
}

/* Check if we have full valid var, an isolated var-sym or no var at all.
 * Return values:
 * 	0: no var at all,
 * 	1: isolated var
 * 	2: full var*/
static int	is_full_var(char *inp, int len, int pos)
{
	if (inp[pos] == '$')
	{
		if (len - pos == 1)
			return (1);
		if (len - pos > 1 && is_valid_varname_char(inp[pos + 1]))
			return (2);
		else
			return (1);
	}
	return (0);
}

void	handle_full_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (is_full_var(cl->input, cl->length, cl->pos) == 2)
	{
		tok->type = TOK_VAR_SYM;
		tok->value = ft_strdup("$");
		nullcheck(tok->value, "handle_full_var()");
		cl->pos++;
		cl->var_flag = 1;
		*tok_found = 1;
	}
}

void	handle_isolated_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		pos;
	char	*inp;

	pos = cl->pos;
	inp = cl->input;
	if (is_full_var(inp, cl->length, pos) == 1)
	{
		if (cl->dquot_flag)
			tok->type = TOK_QWORD;
		else if ((pos + 1 == cl->length) || (pos + 1 < cl->length && \
					inp[pos + 1] != '"' && inp[pos + 1] != '\''))
			tok->type = TOK_WORD;
		tok->value = ft_strdup("$");
		nullcheck(tok->value, "handle_isolated_var()");
		cl->pos++;
		*tok_found = 1;
	}
}

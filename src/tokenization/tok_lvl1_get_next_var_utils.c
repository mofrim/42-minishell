/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_var_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 08:47:18 by elpah             #+#    #+#             */
/*   Updated: 2025/01/13 09:25:21 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_full_var(char *inp, int len, int pos)
{
	if (len - pos > 1 && inp[pos] == '$' && (ft_isalnum(inp[pos + 1]) || \
				inp[pos + 1] == '_'))
		return (1);
	return (0);
}

static int	is_isolated_varsym(char *inp, int len, int pos)
{
	if (len - pos == 1)
		return (1);
	if (len - pos > 1 && inp[pos] == '$' && !(ft_isalnum(inp[pos + 1]) || \
				inp[pos + 1] == '_'))
		return (1);
	return (0);
}

void	handle_full_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (is_full_var(cl->input, cl->length, cl->pos))
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
	if (is_isolated_varsym(cl->input, cl->length, cl->pos))
	{
		if (cl->dquot_flag)
			tok->type = TOK_QWORD;
		else
			tok->type = TOK_VAR_SYM;
		if (!(ft_isalnum(cl->input[cl->pos + 1]) || \
					cl->input[cl->pos + 1] == '_'))
			tok->value = ft_strdup("$");
		else
			tok->value = ft_strdup("");
		nullcheck(tok->value, "handle_isolated_var()");
		cl->pos++;
		*tok_found = 1;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:18:10 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/11 23:49:28 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found);
static char	*get_var_value_from_env(char	*name, t_envlst *env);
static int	is_full_var(char *inp, int len, int pos);
static int	is_isolated_varsym(char *inp, int len, int pos);

void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found && !cl->squot_flag && !cl->var_flag)
	{
		if (is_full_var(cl->input, cl->length, cl->pos))
		{
			tok->type = TOK_VAR_SYM;
			tok->value = ft_strdup("$");
			nullcheck(tok->value, "get_tok_var()");
			cl->pos++;
			cl->var_flag = 1;
			*tok_found = 1;
		}
		else if (is_isolated_varsym(cl->input, cl->length, cl->pos))
		{
			tok->type = TOK_WORD;
			if (ft_isspace(cl->input[cl->pos + 1]))
				tok->value = ft_strdup("$");
			else
				tok->value = ft_strdup("");
			nullcheck(tok->value, "get_tok_var()");
			cl->pos++;
			*tok_found = 1;
		}
	}
}

void	get_tok_var_name(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*tmp;

	if (!*tok_found && cl->var_flag)
	{
		get_tok_word(tok, cl, tok_found);
		tmp = tok->value;
		tok->value = get_var_value_from_env(tok->value, cl->env);
		if (cl->dquot_flag)
			tok->type = TOK_QWORD;
		free(tmp);
		cl->var_flag = 0;
	}
}

static int	is_full_var(char *inp, int len, int pos)
{
	if (len - pos > 1 && inp[pos] == '$' && inp[pos + 1] != '$' && \
			inp[pos + 1] != '"' && inp[pos + 1] != '\'' && \
			!ft_isspace(inp[pos + 1]))
		return (1);
	return (0);
}

static int	is_isolated_varsym(char *inp, int len, int pos)
{
	if (len - pos == 1)
		return (1);
	if (len - pos > 1 && inp[pos] == '$' && (inp[pos + 1] == '$' || \
				inp[pos + 1] == '"' || inp[pos + 1] == '\'' || \
				ft_isspace(inp[pos + 1])))
		return (1);
	return (0);
}

static char	*get_var_value_from_env(char *name, t_envlst *env)
{
	char	*var;

	var = get_env_value(name, env);
	if (!var)
		return (ft_strdup(""));
	return (ft_strdup(var));
}

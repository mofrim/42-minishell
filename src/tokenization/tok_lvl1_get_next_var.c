/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:18:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/30 20:41:25 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found);
static char	*get_var_value_from_env(char	*name, t_envlst *env);

void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found && !cl->squot_flag && !cl->var_flag)
	{
		if (cl->input[cl->pos] == '$')
		{
			tok->type = TOK_VAR_SYM;
			tok->value = ft_strdup("$");
			nullcheck(tok->value, "get_tok_var()");
			cl->pos++;
			cl->var_flag = 1;
			*tok_found = 1;
		}
	}
	if (!*tok_found && cl->var_flag)
	{
		get_tok_word(tok, cl, tok_found);
		if (cl->dquot_flag)
			tok->type = TOK_WORD;
		else
			tok->type = TOK_WORD;
		tok->value = get_var_value_from_env(tok->value, cl->env);
		cl->var_flag = 0;
	}
}

static char	*get_var_value_from_env(char *name, t_envlst *env)
{
	char	*var;

	var = get_env_value(name, env);
	if (!var)
		return (ft_strdup(""));
	return (ft_strdup(var));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:18:10 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/19 08:21:45 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_value_from_env(char *name, t_envlst *env);
void		handle_full_var(t_token *tok, t_cmdline *cl, int *tok_found);
void		handle_isolated_var(t_token *tok, t_cmdline *cl, int *tok_found);
void		get_var_name(t_token *tok, t_cmdline *cl, int *tok_found);

/* Tokenize and kind of parse a var.
 *
 * The basic rules for var parsing / tokenizing in bash are var names can
 * only consist of alphanumeric characters and underscores. Everything else like
 * "$..." cannot be a variable! So in this case we will have a isolated
 * var-symbol. That's all. */
void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found)
{
	if (!*tok_found && !cl->squot_flag && !cl->var_flag)
	{
		handle_full_var(tok, cl, tok_found);
		if (*tok_found)
			return ;
		handle_isolated_var(tok, cl, tok_found);
	}
}

/* If char is a special varname (for now only '?', and single digit numbers
 * $[0-9]), return that as a string. Else NULL. */
static char	*get_special_varname(char c)
{
	if (c == '?')
		return (ft_strdup("?"));
	if (ft_isnum(c))
		return(ft_itoa(c - '0'));
	return (NULL);
}

/* Get the var value. Special var have priority! */
void	get_tok_var_value(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*tmp;

	if (!*tok_found && cl->var_flag)
	{
		tmp = get_special_varname(cl->input[cl->pos]);
		if (tmp)
		{
			tok->value = ft_strdup(tmp);
			cl->pos++;
			*tok_found = 1;
			free(tmp);
		}
		else
			get_var_name(tok, cl, tok_found);
		tmp = tok->value;
		tok->value = get_var_value_from_env(tok->value, cl->env);
		if (cl->dquot_flag)
			tok->type = TOK_QWORD;
		else
			tok->type = TOK_WORD;
		free(tmp);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_tokenize.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 10:57:29 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/02 17:44:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	apply_lvl2_tokenization(t_token *cur, t_token *next);

static t_toktype	is_cmd_or_builtin(t_token *tok);

/* Tokenization Level 2. Goal is to classify all the TOK_WORD tokens and rule
 * out some invalid syntax like
 *
 * 	- "... < < ... "
 * 	- "... | <"
 * 	- "<< |"
 * 	- "<< <"
 * 	- "<< >"
 * 	- a lot more
 *
 * 	Rulez:
 * 	1) after >, <, >>, and << there HAS to be a TOK_WORD!
 * 	2) VAR_NAMES cannot contain |, <, >, <<, >>, ", '
 * 	3) | cannot be the first token
 */
int	tokenize_lvl2(t_tokenlist	*toklst)
{
	t_token	*cur;
	t_token	*next;

	if (!check_toklst_lvl2(toklst))
		return (0);
	cur = toklst->token;
	if (cur->type == TOK_WORD)
		cur->type = is_cmd_or_builtin(cur);
	while (toklst->next)
	{
		next = toklst->next->token;
		apply_lvl2_tokenization(cur, next);
		cur = next;
		toklst = toklst->next;
	}
	return (1);
}

static t_toktype	is_cmd_or_builtin(t_token *tok)
{
	char	*val;

	val = tok->value;
	if (!ft_strcmp(val, "echo") || !ft_strcmp(val, "cd") || \
			!ft_strcmp(val, "pwd") || !ft_strcmp(val, "export") || \
			!ft_strcmp(val, "unset") || !ft_strcmp(val, "env") || \
			!ft_strcmp(val, "exit"))
		return (TOK_BLTIN);
	return (TOK_CMD);
}

static void	apply_lvl2_tokenization(t_token *cur, t_token *next)
{
	if (cur->type == TOK_RIN && next->type == TOK_WORD)
		next->type = TOK_IF;
	if ((cur->type == TOK_ROUT || cur->type == TOK_ROUTA) && \
			next->type == TOK_WORD)
		next->type = TOK_OF;
	if ((cur->type == TOK_IF || cur->type == TOK_OF || \
				cur->type == TOK_PIP) && next->type == TOK_WORD)
		next->type = is_cmd_or_builtin(next);
	if (cur->type == TOK_CMD && next->type == TOK_WORD)
		next->type = TOK_ARG;
	if (cur->type == TOK_BLTIN && next->type == TOK_WORD)
		next->type = TOK_BLTIN_ARG;
	if (cur->type == TOK_ARG && next->type == TOK_WORD)
		next->type = TOK_ARG;
	if (cur->type == TOK_BLTIN_ARG && next->type == TOK_WORD)
		next->type = TOK_BLTIN_ARG;
	if (cur->type == TOK_HERE && next->type == TOK_WORD)
		next->type = TOK_HERE_DLIM;
	if (cur->type == TOK_VAR_SYM && next->type == TOK_WORD)
		next->type = TOK_VAR_NAME;
}


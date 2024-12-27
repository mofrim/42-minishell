/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_redir_in.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:16:04 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 18:08:37 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// + implement `[n]<file` as opening `file` for reading as fildes `n`.
//   TOK_RIN1. `cat 42<` is TOKERR_NL!!

void	get_tok_rin(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*inp;
	int		pos;

	inp = cl->input;
	pos = cl->pos;
	if (!*tok_found)
	{
		if (pos >= 1 && inp[pos] == '<' && !ft_isnum(inp[pos - 1]))
		{
			tok->type = TOK_RIN0;
			tok->value = ft_strdup("<");
			nullcheck(tok->value, "get_tok_rin()");
			cl->pos++;
			*tok_found = 1;
		}
		else if (pos >= 1 && inp[pos] == '<' && ft_isnum(inp[pos - 1]))
		{
			tok->type = TOK_RIN1;
			tok->value = ft_strdup("<");
			nullcheck(tok->value, "get_tok_rin()");
			cl->pos++;
			*tok_found = 1;
		}
	}
}

// + maybe implement `<> file` for opening stdin and stdout to `file`
void	get_tok_rinout(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*inp;
	int		pos;

	inp = cl->input;
	pos = cl->pos;
	if (!*tok_found)
	{
		if (pos >= 1 && cl->length - pos > 1 && ft_isnum(inp[pos - 1]) && \
				!ft_strncmp(&inp[pos], "<>", 2))
		{
			tok->type = TOK_RINOUT1;
			tok->value = ft_strdup("<>");
			nullcheck(tok->value, "get_tok_rinout()");
			cl->pos += 2;
			*tok_found = 1;
		}
		else if (cl->length - pos > 1 && !ft_strncmp(&inp[pos], "<>", 2))
		{
			tok->type = TOK_RINOUT0;
			tok->value = ft_strdup("<>");
			nullcheck(tok->value, "get_tok_rinout()");
			cl->pos += 2;
			*tok_found = 1;
		}
	}
}

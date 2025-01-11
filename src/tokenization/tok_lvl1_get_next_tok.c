/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_tok.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:55:17 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/12 00:33:31 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Would have made them all static if there wasn't normi. Still don't want them
 * in my minishell.h because they are not of global interest for the proj. So,
 * here are the prototypes needed in here: */
void	get_tok_pipe(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_rin(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_rinout(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_rout(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_here(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_quot(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_word(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_squotword(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_dquotword(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_var(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_var_name(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_redir_outa(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_redir_out12(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_redir_out3(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_and(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_emptyquot(t_token *tok, t_cmdline *cl, int *tok_found);
void	get_tok_white(t_token *tok, t_cmdline *cl, int *tok_found);

/* The order in this function is extremly relevant! */
static void	get_tok_unquoted(t_token *tok, t_cmdline *cl, int *tok_found)
{
	get_tok_white(tok, cl, tok_found);
	get_tok_emptyquot(tok, cl, tok_found);
	get_tok_quot(tok, cl, tok_found);
	get_tok_var(tok, cl, tok_found);
	get_tok_var_name(tok, cl, tok_found);
	get_tok_pipe(tok, cl, tok_found);
	get_tok_rinout(tok, cl, tok_found);
	get_tok_redir_outa(tok, cl, tok_found);
	get_tok_redir_out3(tok, cl, tok_found);
	get_tok_redir_out12(tok, cl, tok_found);
	get_tok_rout(tok, cl, tok_found);
	get_tok_here(tok, cl, tok_found);
	get_tok_rin(tok, cl, tok_found);
	get_tok_and(tok, cl, tok_found);
	get_tok_word(tok, cl, tok_found);
}

static void	get_tok_quoted(t_token *tok, t_cmdline *cl, int *tok_found)
{
	get_tok_quot(tok, cl, tok_found);
	get_tok_var_name(tok, cl, tok_found);
	get_tok_var(tok, cl, tok_found);
	get_tok_squotword(tok, cl, tok_found);
	get_tok_dquotword(tok, cl, tok_found);
}

// TODO: if there was input like `blalba"asdsd"` so there was no whitespace in
// between preceeding or following text and a quoted string both strings should
// be joined later on.
t_token	*get_next_token(t_cmdline *cl)
{
	t_token	*token;
	int		tok_found;

	// if (!cl->dquot_flag && !cl->squot_flag)
	// 	skip_whitespace(cl);
	if (cl->pos >= cl->length)
		return (NULL);
	token = init_token(NULL, TOK_NULL);
	nullcheck(token, "get_next_token()");
	tok_found = 0;
	while (cl->pos < cl->length && !tok_found)
	{
		if (!cl->dquot_flag && !cl->squot_flag)
			get_tok_unquoted(token, cl, &tok_found);
		else
			get_tok_quoted(token, cl, &tok_found);
	}
	return (token);
}

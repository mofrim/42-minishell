/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_redir_out.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 21:44:21 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 14:32:43 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isnum(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

/* Find possible token candidates of type [n]>word or [n]&>[n]. 
 * Possible tokens:
 *
 * TOK_ROUT0:	simple '> word'
 * TOK_ROUT1:	'[n]> word'. word is always TOK_OF! no fildes num exansion!
 * TOK_ROUT2:	'&>word'. this is the only legit sequence involving &>! in
 * 				'2&>1' the '2' is counted as an arg and the outfile will be 
 * 				called '1'! redir stdout & stderr to word.
 * TOK_ROUT3:	'[n]>&word'... let bash-man speak itself ->
 *
 * 		The operator
 * 					[n]>&word
 * 		is used similarly to duplicate output file descriptors.  If n is not
 * 		specified, the standard output (file descriptor 1) is used.  If the
 * 		digits in word do not specify a file descriptor open for output, a
 * 		redirection error occurs.  If word evaluates to -, file descriptor n is
 * 		closed.  As a special case, if n is  omitted,  and  word does not expand
 * 		to one or more digits or -, the standard output and standard error are
 * 		redirected as described previously.
 *
 * 		So. `echo "bla" >&file` is correct but will act just like `>`. But `echo
 * 		"bla" 2>&file` is an error.
 *
 * This will be it!!!
 *
 */
// 1) n>&word: -> TOK_ROUT3, if word not valid fildes -> error
// 2) >&word: word = digits, redir stdout to fildes -> TOK_ROUT3
// 3) >&word: word != digits, redir stdouterr to file word -> TOK_ROUT2
void	get_tok_redir_out(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*inp;
	int		pos;

	inp = cl->input;
	pos = cl->pos;
	if (!*tok_found)
	{
		if (pos >= 1 && cl->length - pos > 1 && inp[pos] == '>' && \
				inp[pos + 1] != '&' && ft_isnum(inp[pos - 1]))
		{
			tok->type = TOK_ROUT1;
			tok->value = ft_strdup(">");
			cl->pos++;
			*tok_found = 1;
		}
		else if (pos >= 1 && cl->length - pos > 2 && \
				!ft_strncmp(&inp[pos], "&>", 2))
		{
			tok->type = TOK_ROUT2;
			tok->value = ft_strdup("&>");
			cl->pos += 2;
			*tok_found = 1;
		}
		else if (pos >= 1 && cl->length - pos > 2 && ft_isnum(inp[pos - 1]) && \
				ft_isnum(inp[pos + 2]) && !ft_strncmp(&inp[pos], ">&", 2))
		{
			tok->type = TOK_ROUT3;
			tok->value = ft_strdup(">&");
			cl->pos += 2;
			*tok_found = 1;
		}
		// if '[n]>&bla' -> this would be an error. if 'bla>&[n]' with n being
		// valid fd this would dup stdout to fd n, if fd n not valid -> "Bad
		// fd". but still both are to be treated like TOK_ROUT3
		else if (pos >= 1 && cl->length - pos > 2 && (!ft_isnum(inp[pos - 1]) || \
				!ft_isnum(inp[pos + 2])) && !ft_strncmp(&inp[pos], ">&", 2))
		{
			tok->type = TOK_ROUT3;
			tok->value = ft_strdup(">&");
			cl->pos += 2;
			*tok_found = 1;
		}
		// if 'bla>&moep' act like '&>'.
		else if (pos >= 1 && cl->length - pos > 2 && !ft_isnum(inp[pos - 1]) \
				&& !ft_isnum(inp[pos + 2]) && !ft_strncmp(&inp[pos], ">&", 2))
		{
			tok->type = TOK_ROUT2;
			tok->value = ft_strdup(">&");
			cl->pos += 2;
			*tok_found = 1;
		}
	}
}

/* From the bash man:
 *
 * The format for appending standard output and standard error is:
 *               &>>word
 *        This is semantically equivalent to
 *               >>word 2>&1
 *
 * So, nothing more and nothing less we want to achieve here. Therefore we need
 * 3 different tokens:
 * 
 * TOK_ROUTA0: simple 'word >> word'
 * TOK_ROUTA1: 'n>> word'
 * TOK_ROUTA2:	'&>>word'
 *
 * There is no '[n]&>>word' for appending!
 */
void	get_tok_redir_outa(t_token *tok, t_cmdline *cl, int *tok_found)
{
	char	*inp;
	int		pos;

	inp = cl->input;
	pos = cl->pos;
	if (!*tok_found)
	{
		if (pos >= 1 && cl->length - pos > 1 && ft_isnum(inp[pos - 1]) && \
				!ft_strncmp(&inp[pos], ">>", 2))
		{
			tok->type = TOK_ROUTA1;
			tok->value = ft_strdup(">>");
			cl->pos += 2;
			*tok_found = 1;
		}
		else if (cl->length - pos > 3 && !ft_strncmp(&inp[pos], "&>>", 3))
		{
			tok->type = TOK_ROUTA2;
			tok->value = ft_strdup("&>>");
			cl->pos += 3;
			*tok_found = 1;
		}
	}
}

/* Sorry for the hacky onemore flag 8) */
void	get_tok_rout(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int	onemore;

	onemore = cl->length - cl->pos > 1;
	if (!*tok_found)
	{
		if (cl->pos < cl->length - onemore && cl->input[cl->pos] == '>')
		{
			if (!onemore || cl->input[cl->pos + onemore] != '>')
			{
				tok->type = TOK_ROUT0;
				tok->value = ft_strdup(">");
				nullcheck(tok->value, "get_tok_rout()");
				cl->pos++;
				*tok_found = 1;
			}
			if (onemore && cl->input[cl->pos + onemore] == '>')
			{
				tok->type = TOK_ROUTA0;
				tok->value = ft_strdup(">>");
				nullcheck(tok->value, "get_tok_rout()");
				cl->pos += 2;
				*tok_found = 1;
			}
		}
	}
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_next_varname.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:37:06 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/22 13:01:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Get the envvar's name from input.
 *
 * Needed to use this specialized get_tok_word, bc there is at least one more
 * extra stopping condition for var names: at '/'. But acutally we could define
 * this as in bash to just continue until char != alnum or '_'. We'll see...
 */
void	get_var_name(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		start;
	int		length;
	char	*word;

	if (!*tok_found)
	{
		start = cl->pos;
		while (cl->pos < cl->length && !ft_isspace(cl->input[cl->pos]) && \
				is_valid_varname_char(cl->input[cl->pos]))
			cl->pos++;
		length = cl->pos - start;
		word = malloc(length + 1);
		nullcheck(word, "get_var_name()");
		ft_strlcpy(word, &cl->input[start], length + 1);
		tok->value = word;
		tok->type = TOK_WORD;
		*tok_found = 1;
	}
}

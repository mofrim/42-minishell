/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl2_split_vwords.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 20:25:07 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/28 22:50:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Add the split up token to our to-be-inserted toklst. Seperate each token by a
 * TOK_WHITE. */
static void	add_splittok_to_splitlst(char *svalue, t_toklst **tlst)
{
	t_token		*new;
	t_token		*white;

	white = init_token(" ", TOK_WHITE);
	toklst_add_back(tlst, toklst_new(white));
	new = init_token(svalue, TOK_WORD);
	toklst_add_back(tlst, toklst_new(new));
}

/* Split the tokenstring and return a toklst containing the split. */
t_toklst	*split_vword_token(char *vword)
{
	t_toklst	*splitlst;
	char		**split_strings;
	int			i;
	int			nsplits;

	splitlst = NULL;
	split_strings = ft_multisplit(vword, " \t\v\n\f", ft_strlen(" \t\v\n\f"));
	i = 0;
	nsplits = splitsize(split_strings);
	if (nsplits == 0)
		return (toklst_add_back(&splitlst, toklst_new_tok(TOK_WHITE, " ")),
			free_ptr2ptr(&split_strings), splitlst);
	if (ft_isspace(vword[0]))
		toklst_add_back(&splitlst, toklst_new_tok(TOK_WHITE, " "));
	toklst_add_back(&splitlst, toklst_new_tok(TOK_WORD, split_strings[0]));
	while (++i < nsplits)
		add_splittok_to_splitlst(split_strings[i], &splitlst);
	if (ft_strlen(vword) > 1 && ft_isspace(vword[ft_strlen(vword) - 1]))
		toklst_add_back(&splitlst, toklst_new(init_token(" ", TOK_WHITE)));
	free_ptr2ptr(&split_strings);
	return (splitlst);
}

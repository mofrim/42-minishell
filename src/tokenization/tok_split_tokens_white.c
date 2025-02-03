/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_split_words_white.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:52:18 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:13:49 by fmaurer          ###   ########.fr       */
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
	new = init_token(svalue, TOK_ARG);
	toklst_add_back(tlst, toklst_new(new));
}

/* Split the tokenstring and return a toklst containing the split. */
static t_toklst	*split_cmdbltin_token(char *cmdstr)
{
	t_toklst	*splitlst;
	char		**split_strings;
	int			i;
	int			nsplits;

	splitlst = NULL;
	split_strings = ft_multisplit(cmdstr, " \t\v\n\f", ft_strlen(" \t\v\n\f"));
	i = 0;
	nsplits = splitsize(split_strings);
	if (nsplits == 0)
		return (toklst_add_back(&splitlst, toklst_new_tok(TOK_CMD, " ")), \
				free_ptr2ptr(&split_strings), splitlst);
	toklst_add_back(&splitlst, toklst_new_tok(
			is_cmd_or_builtin(split_strings[0]), split_strings[0]));
	while (++i < nsplits)
		add_splittok_to_splitlst(split_strings[i], &splitlst);
	free_ptr2ptr(&split_strings);
	return (splitlst);
}

/* Insert the split token into the toklst at the same position where it was
 * before. */
static void	insert_splittoken_into_toklst(t_toklst *splitcmd_toklst,
		t_toklst **tl, t_toklst **tlst)
{
	t_toklst	*next;

	next = (*tl)->next;
	if (toklst_size(*tlst) == 1)
	{
		*tlst = splitcmd_toklst;
		free_one_toklst(*tl);
		*tl = NULL;
		return ;
	}
	if ((*tl)->prev)
		(*tl)->prev->next = splitcmd_toklst;
	else
		*tlst = splitcmd_toklst;
	if (next)
	{
		toklst_last(splitcmd_toklst)->next = next;
		next->prev = toklst_last(splitcmd_toklst);
	}
	free_one_toklst(*tl);
	*tl = next;
}

/**
 * Split up VWORD, CMD or BLTIN tokens which contain whitespaces.
 *
 * Split up any TOK_CMD or TOK_BLTIN that contains whitespaces into
 * - first word becomes a TOK_BLTIN / TOK_CMD
 * - following words become TOK_ARGs
 *
 * Will modify the toklst pointer given as a parameter so that it points to the
 * new toklst if the head-pointer was split.
 *
 * This is also the way bash handles it. try with 
 * 		`export bla="ls --color > blub"` 
 * you will see the redirection is not expanded but interpreted as an arg.
 *
 * At the end of the main loop we do a step which is only needed if this
 * function gets called in tok-lvl3: making any TOK_QCMD -> TOK_CMD. This
 * actually should not be done in here.
 */
void	split_tokens_with_whitespaces(t_toklst **tlst)
{
	t_toklst	*tl;
	t_token		*tok;
	t_toklst	*split_toklst;

	tl = *tlst;
	while (tl)
	{
		tok = tl->token;
		if ((tok->type == TOK_CMD || tok->type == TOK_BLTIN || \
				tok->type == TOK_VWORD) && has_whitespace(tok->value))
		{
			if (tok->type == TOK_VWORD)
				split_toklst = split_vword_token(tok->value);
			else
				split_toklst = split_cmdbltin_token(tok->value);
			insert_splittoken_into_toklst(split_toklst, &tl, tlst);
		}
		else
		{
			if (tok->type == TOK_QCMD)
				tok->type = TOK_CMD;
			if (tl)
				tl = tl->next;
		}
	}
}

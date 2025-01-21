/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_split_words_white.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:52:18 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/21 13:43:14 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Return 1 if we have a whitspace char in string str. Else 0. */
static int	has_whitespace(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

/* Add the split up token to our to-be-inserted toklst. Seperate each token by a
 * TOK_WHITE. */
static void	add_splittok_to_splitlst(char *svalue, int orig_ttype,
		t_toklst **tlst, int nsplitsi[2])
{
	t_token		*new;
	t_token		*white;
	int			i;
	int			nsplits;

	nsplits = nsplitsi[0];
	i = nsplitsi[1];
	if (i == 1)
	{
		white = init_token(" ", TOK_WHITE);
		toklst_add_back(tlst, toklst_new(white));
	}
	if (orig_ttype == TOK_CMD || orig_ttype == TOK_BLTIN)
		new = init_token(svalue, TOK_ARG);
	else
		new = init_token(svalue, TOK_WORD);
	toklst_add_back(tlst, toklst_new(new));
	if (i < nsplits)
	{
		white = init_token(" ", TOK_WHITE);
		toklst_add_back(tlst, toklst_new(white));
	}
}

/* Split the tokenstring and return a toklst containing the split. */
static t_toklst	*split_token(char *cmdstr, t_toktype ttype)
{
	t_toklst	*splitlst;
	t_token		*new;
	char		**split_strings;
	int			i;
	int			nsplits;

	splitlst = NULL;
	split_strings = ft_multisplit(cmdstr, " \t\v\n\f", ft_strlen(" \t\v\n\f"));
	i = 0;
	nsplits = splitsize(split_strings);
	if (ttype == TOK_CMD || ttype == TOK_BLTIN)
		new = init_token(split_strings[0], is_cmd_or_builtin(split_strings[0]));
	else
		new = init_token(split_strings[0], TOK_WORD);
	toklst_add_back(&splitlst, toklst_new(new));
	while (++i < nsplits)
		add_splittok_to_splitlst(split_strings[i], ttype, &splitlst,
			(int [2]){nsplits, i});
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
 * Split up WORD, CMD or BLTIN tokens which contain whitespaces.
 *
 * Split up any TOK_CMD or TOK_BLTIN that contains whitespaces into
 * - first word becomes a TOK_BLTIN / TOK_CMD
 * - following words become TOK_ARGs
 *
 * Will modify the toklst pointer given as a parameter so that it points to the
 * new toklst if the head-pointer was split.
 *
 * This is also the way bash handles it. try with `export bla="ls --color >
 * blub"` you will see the redirection is not expanded but interpreted as an
 * arg. */
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
				tok->type == TOK_WORD) && has_whitespace(tok->value))
		{
			split_toklst = split_token(tok->value, tok->type);
			insert_splittoken_into_toklst(split_toklst, &tl, tlst);
		}
		else
		{
			if (tok->type == TOK_QCMD)
				tok->type = TOK_CMD;
			tl = tl->next;
		}
	}
}

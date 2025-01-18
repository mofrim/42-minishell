/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_lvl1_get_tok_herdlim.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:37:44 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 22:07:35 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes_from_string(char **str);

static int	is_quot(char c)
{
	if (c == '\'')
		return (1);
	if (c == '"')
		return (2);
	return (0);
}

/* Is metachar? Deliberately not using all bash metachars here because we don't
 * know them in our project. */
static int	is_metachar(char c)
{
	if (c == '|' || c == '&' || c == '>' || c == '<')
		return (1);
	return (0);
}

/**
 * Get the heredoc delim.
 *
 * If heredoc delim is quoted like `<< "$&;"` ignore any metachars or
 * whitespaces in it. Otherwise get everythin until first metachar or
 * whitespace.
 */
static char	*get_dlim(char *inp, int *pos, int cllen, int *quot)
{
	int		start;
	int		dlimlen;
	char	*word;
	int		firstquot;

	start = *pos;
	firstquot = 0;
	while (*pos < cllen && (!ft_isspace(inp[*pos]) || firstquot) && \
			(!is_metachar(inp[*pos]) || firstquot))
	{
		if (is_quot(inp[*pos]) && !firstquot)
		{
			*quot = 1;
			firstquot = is_quot(inp[*pos]);
		}
		else if (firstquot && is_quot(inp[*pos]) == firstquot)
			firstquot = 0;
		(*pos)++;
	}
	dlimlen = *pos - start;
	word = malloc(dlimlen + 1);
	nullcheck(word, "get_tok_herdlim()");
	ft_strlcpy(word, &inp[start], dlimlen + 1);
	return (word);
}

/**
 * Get TOK_HERE_xDLIM.
 *
 * If we have something like `<<|` we stop and leave this syntax error to the
 * check_toklst in lvl2
 */
void	get_tok_herdlim(t_token *tok, t_cmdline *cl, int *tok_found)
{
	int		quot;
	char	*word;

	if (!*tok_found && cl->herdlim_flag)
	{
		if (is_metachar(cl->input[cl->pos]))
		{
			cl->herdlim_flag = 0;
			return ;
		}
		quot = 0;
		word = get_dlim(cl->input, &cl->pos, cl->length, &quot);
		if (quot)
			remove_quotes_from_string(&word);
		tok->value = word;
		if (quot)
			tok->type = TOK_HERE_QDLIM;
		else
			tok->type = TOK_HERE_DLIM;
		cl->herdlim_flag = 0;
		*tok_found = 1;
	}
}

static void	remove_quotes_from_string(char **str)
{
	size_t	i;
	size_t	j;
	size_t	len;
	int		quots;
	char	*quot_free_str;

	quots = 0;
	len = ft_strlen(*str);
	i = -1;
	while (++i < len)
		if ((*str)[i] == '"' || (*str)[i] == '\'')
			quots++;
	quot_free_str = ft_calloc((len - quots + 1), sizeof(char));
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((*str)[i] != '"' && (*str)[i] != '\'')
			quot_free_str[j++] = (*str)[i++];
		else
			i++;
	}
	free(*str);
	*str = quot_free_str;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/21 22:22:12 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize tokenizer with input string. Skip any whitespace chars at the
 * begin. */
t_cmdline* init_cmdline(char* input) 
{
	// TODO: handle malloc
    t_cmdline* tok = malloc(sizeof(t_cmdline));
	while (ft_isspace(*input) && *input)
		input++;
    tok->input = input;
    tok->position = 0;
    tok->length = ft_strlen(input);
    return (tok);
}

t_tokenlist	*tokenize(char *input)
{
	t_cmdline	*cmdline;
	t_tokenlist		*tok_lst;
	t_token			*tmptok;

	cmdline = init_cmdline(input);
	tmptok = get_next_token(cmdline);
	tok_lst = toklst_new(tmptok);
	while (tmptok->type != TOK_EOF)
	{
		tmptok = get_next_token(cmdline);
		if (tmptok->type == TOK_EOF)
		{
			free(tmptok);
			return (tok_lst);
		}
		if (tmptok->type == TOK_ARG && \
				toklst_last(tok_lst)->token->type == TOK_PIP)
			tmptok->type = TOK_CMD;
		if (tmptok->type == TOK_ARG && \
				toklst_last(tok_lst)->token->type == TOK_RIN)
			tmptok->type = TOK_IF;
		if (tmptok->type == TOK_ARG && \
				toklst_last(tok_lst)->token->type == TOK_IF)
			tmptok->type = TOK_CMD;
		if (tmptok->type == TOK_ARG && \
				toklst_last(tok_lst)->token->type == TOK_ROUT)
			tmptok->type = TOK_OF;
		toklst_add_back(&tok_lst, toklst_new(tmptok));
	}
	return (tok_lst);
}

/* Skip whitespace */
static void skip_whitespace(t_cmdline* lexer) {
    while (lexer->position < lexer->length && 
           ft_isspace(lexer->input[lexer->position])) {
        lexer->position++;
    }
}

/* Get next token from input */
// TODO split up ans simplify because there will be more tokentypes to be lexed.
t_token*	get_next_token(t_cmdline* cmdline)
{
    t_token*	token;
	char	current;

	// TODO handle malloc
	token = malloc(sizeof(t_token));

    skip_whitespace(cmdline);
    if (cmdline->position >= cmdline->length)
	{
        token->type = TOK_EOF;
        token->value = NULL;
        return token;
    }
    current = cmdline->input[cmdline->position];
    if (current == '|')
	{
        token->type = TOK_PIP;
        token->value = ft_strdup("|");
        cmdline->position++;
        return token;
    }
    if (current == '>') 
	{
        token->type = TOK_ROUT;
        token->value = ft_strdup(">");
        cmdline->position++;
        return token;
    }
    if (current == '<') 
	{
        token->type = TOK_RIN;
        token->value = ft_strdup("<");
        cmdline->position++;
        return token;
    }
    /* Read word (command, argument or in/outfile) */
	int start = cmdline->position;
	while (cmdline->position < cmdline->length && \
			!ft_isspace(cmdline->input[cmdline->position]) && \
			cmdline->input[cmdline->position] != '|' && \
			cmdline->input[cmdline->position] != '>' &&\
			cmdline->input[cmdline->position] != '<')
		cmdline->position++;
    
    int length = cmdline->position - start;
    char* word = malloc(length + 1);
    ft_strlcpy(word, &cmdline->input[start], length + 1);
    word[length] = '\0';
    
	/* First word is always a command. As long as we don't implement '<'. */
	if (start == 0)
		token->type = TOK_CMD;
	else
		token->type = TOK_ARG;
	token->value = word;

    return (token);
}

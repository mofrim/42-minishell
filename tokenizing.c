/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 23:57:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize tokenizer with input string. Skip any whitespace chars at the
 * begin. */
t_inputstream* init_inputstream(char* input) 
{
	// TODO: handle malloc
    t_inputstream* tok = malloc(sizeof(t_inputstream));
	while (ft_isspace(*input) && *input)
		input++;
    tok->input = input;
    tok->position = 0;
    tok->length = ft_strlen(input);
    return (tok);
}

t_tokenlist	*tokenize(char *input)
{
	t_inputstream	*input_stream;
	t_tokenlist		*tok_lst;
	t_token			*tmptok;

	input_stream = init_inputstream(input);
	tmptok = get_next_token(input_stream);
	tok_lst = toklst_new(tmptok);
	while (tmptok->type != TOKEN_EOF)
	{
		tmptok = get_next_token(input_stream);
		if (tmptok->type == TOKEN_EOF)
		{
			free(tmptok);
			return (tok_lst);
		}
		if (tmptok->type == TOKEN_ARG && \
				toklst_last(tok_lst)->token->type == TOKEN_PIPE)
			tmptok->type = TOKEN_COMMAND;
		toklst_add_back(&tok_lst, toklst_new(tmptok));
	}
	return (tok_lst);
}

/* Skip whitespace */
static void skip_whitespace(t_inputstream* lexer) {
    while (lexer->position < lexer->length && 
           ft_isspace(lexer->input[lexer->position])) {
        lexer->position++;
    }
}

/* Get next token from input */
// TODO split up ans simplify because there will be more tokentypes to be lexed.
t_token*	get_next_token(t_inputstream* lexer) 
{
    t_token*	token;
	char	current;

	// TODO handle malloc
	token = malloc(sizeof(t_token));

    skip_whitespace(lexer);
    if (lexer->position >= lexer->length) {
        token->type = TOKEN_EOF;
        token->value = NULL;
        return token;
    }
    current = lexer->input[lexer->position];
    if (current == '|') 
	{
        token->type = TOKEN_PIPE;
        token->value = ft_strdup("|");
        lexer->position++;
        return token;
    }
    if (current == '>') 
	{
        token->type = TOKEN_REDIRECT;
        token->value = ft_strdup(">");
        lexer->position++;
        return token;
    }

    /* Read word (command or argument) */
    int start = lexer->position;
    while (lexer->position < lexer->length && 
           !ft_isspace(lexer->input[lexer->position]) && 
           lexer->input[lexer->position] != '|' && 
           lexer->input[lexer->position] != '>') {
        lexer->position++;
    }
    
    int length = lexer->position - start;
    char* word = malloc(length + 1);
    ft_strlcpy(word, &lexer->input[start], length + 1);
    word[length] = '\0';
    
	/* First word is always a command. As long as we don't implement '<'. */
	if (start == 0)
		token->type = TOKEN_COMMAND;
	else
		token->type = TOKEN_ARG;
	token->value = word;

    return (token);
}

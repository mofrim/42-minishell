/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 22:23:01 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "minishell.h"

/* Initialize parser */
Parser* init_parser(t_inputstream* lexer) {
    Parser* parser = malloc(sizeof(Parser));
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    return parser;
}

/* Create new command structure */
Command* new_command(char* executable) {
    Command* cmd = malloc(sizeof(Command));
    cmd->executable = ft_strdup(executable);
    cmd->args = malloc(sizeof(char*) * 10);  // Start with space for 10 args
    cmd->arg_count = 0;
    cmd->next = NULL;
    cmd->output_file = NULL;
    return cmd;
}

/* Parse input into command structure */
Command* parse_cmdline(Parser* parser) {
    Command* first_cmd = NULL;
    Command* current_cmd = NULL;

    while (parser->current_token->type != TOKEN_EOF)
	{
		if (parser->current_token->type == TOKEN_COMMAND) 
		{
			// ft_printf("DEBUG: parse -> found TOKEN_COMMAND\n");
			Command* cmd = new_command(parser->current_token->value);

			if (!first_cmd) first_cmd = cmd;
			if (current_cmd) current_cmd->next = cmd;
			current_cmd = cmd;

			parser->current_token = get_next_token(parser->lexer);

			/* Parse arguments */
			while (parser->current_token->type == TOKEN_ARG) {
				// ft_printf("DEBUG: parse -> found TOKEN_ARG\n");
				cmd->args[cmd->arg_count++] = ft_strdup(parser->current_token->value);
				parser->current_token = get_next_token(parser->lexer);
			}
		}
		else if (parser->current_token->type == TOKEN_PIPE) 
		{
			// ft_printf("DEBUG: parse -> found TOKEN_ARG\n");
			parser->current_token = get_next_token(parser->lexer);
		} 
		else if (parser->current_token->type == TOKEN_REDIRECT) 
		{
			parser->current_token = get_next_token(parser->lexer);
			if (parser->current_token->type == TOKEN_ARG) 
			{
				current_cmd->output_file = ft_strdup(parser->current_token->value);
				parser->current_token = get_next_token(parser->lexer);
			}
		}
	}
    return first_cmd;
}

/* Print command structure (for debugging) */
void print_command(Command* cmd) {
    while (cmd) {
        printf("Command: %s\n", cmd->executable);
        printf("Arguments: ");
        for (int i = 0; i < cmd->arg_count; i++) {
            printf("%s ", cmd->args[i]);
        }
        printf("\n");
        
        if (cmd->output_file) {
            printf("Output redirected to: %s\n", cmd->output_file);
        }
        
        if (cmd->next) {
            printf("Piped to:\n");
        }
        
        cmd = cmd->next;
    }
}

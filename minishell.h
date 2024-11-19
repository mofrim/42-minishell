/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 21:56:26 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include <signal.h>

/* For ???? */
#include <stdio.h>

# define PROMPT "$"

typedef struct termios t_termios;

/*********** Datatypes for tokenization. ***********/

/* Token types */
// TODO: implement <, <<, >>
typedef enum {
    TOKEN_COMMAND,    // executable or command
    TOKEN_ARG,        // command argument or file
    TOKEN_PIPE,       // |
    TOKEN_REDIRECT,   // >
    TOKEN_EOF,        // end of input
} e_tokentype;

/* Token structure */
typedef struct {
    e_tokentype type;
    char* value;
} t_token;

/* Inputstream structure */
typedef struct {
    char	*input;
    int		position;
    int		length;
} t_inputstream;

typedef struct s_tokenlist {
	t_token *token;
	struct s_tokenlist *next;
} t_tokenlist;

/*********** Datatypes for parsing. ***********/

/* Command structure for parser */
typedef struct Command {
    char* executable;
    char** args;
    int arg_count;
    struct Command* next;    // next command in pipeline
    char* output_file;       // file for redirection
} Command;


/* Parser structure */
typedef struct {
    t_inputstream* lexer;
    t_token* current_token;
} Parser;

typedef struct s_cmdline
{
	char **cmdl_split;
} t_cmdline;

void	signal_handler(int signum);
void	signal_setup(void (*sig_handler)(int));
void	term_setup(struct termios *old_settings);

void	nullcheck(void *p, char *msg);
void	error_exit(char *msg);

int	ft_isspace(char c);

Parser* init_parser(t_inputstream* lexer);
Command* new_command(char* executable);
Command* parse_cmdline(Parser* parser);
void print_command(Command* cmd);
t_inputstream* init_inputstream(char* input);
t_token* get_next_token(t_inputstream* lexer);

/* tokenlist stuff. */

t_tokenlist	*toklst_new(t_token *tok);
t_tokenlist	*toklst_last(t_tokenlist *head);
void	toklst_add_back(t_tokenlist **head, t_tokenlist *newend);
void	toklst_clear(t_tokenlist **lst);
int	toklst_size(t_tokenlist *lst);

t_tokenlist	*tokenize(char *input);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/21 21:30:14 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"

/* For ??? */
#include <signal.h>

/* For ???? */
#include <stdio.h>

# define PROMPT "$"

/* Define termios type for easier reference. */
typedef struct termios t_termios;

/*********** Datatypes for tokenization. ***********/

/* The highest Token Number. Useful for iterating over Tokens, maybe?! */
# define TOKEN_MAX 18

/* Token types... are these really all? */
typedef enum {
    TOK_CMD			= 0,	// executable or command
    TOK_ARG			= 1,	// command argument or file
    TOK_PIP			= 2,	// |
	TOK_ROUT		= 3,	// >
	TOK_RIN			= 4,	// <
	TOK_OF			= 5,	// outfile after a >
	TOK_IF			= 6,	// infile after a <
	TOK_ROUTA		= 7,	// >>
	TOK_HERE		= 8,	// <<
	TOK_HERE_DLIM	= 9,	// << delimiter
	TOK_SQUOT		= 10,	// single quote
	TOK_DQUOT		= 11,	// double quote
	TOK_SQUOT_TXT	= 12,	// single quote text
	TOK_DQUOT_TXT	= 13,	// double quote text
	TOK_VAR_SYM		= 14,	// variable symbol
	TOK_VAR_NAME	= 15,	// variable name
	TOK_BLTIN		= 16,	// builtin command
	TOK_BLTIN_ARG	= 17,	// builtin command argument
    TOK_EOF			= 18,	// end of input
} e_tokentype;

/* Token structure */
typedef struct {
    e_tokentype	type;
    char		*value;
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
typedef struct s_cmdlst {
    char	*executable;
    char	**args;
    int		arg_count;
    char	*output_file;       // file for redirection
    char	*input_file;       // file for redirection
    struct	s_cmdlst* next;    // next command in pipeline
} t_cmdlst;

/* Parser structure */
typedef struct {
    t_inputstream* inputstream;
    t_token* current_token;
} t_parser;

/*********** Signal and terminal setup. ***********/

void	signal_handler(int signum);
void	signal_setup(void (*sig_handler)(int));
void	term_setup(struct termios *old_settings);

/*********** Utils ***********/

void	nullcheck(void *p, char *msg);
void	error_exit(char *msg);
int		ft_isspace(char c);

/*********** Tokenization. ***********/

t_tokenlist	*toklst_new(t_token *tok);
t_tokenlist	*toklst_last(t_tokenlist *head);
void	toklst_add_back(t_tokenlist **head, t_tokenlist *newend);
void	toklst_clear(t_tokenlist **lst);
int	toklst_size(t_tokenlist *lst);
t_tokenlist	*tokenize(char *input);

/*********** Parsing. ***********/

t_parser*		init_parser(t_inputstream* lexer);
t_cmdlst*		new_command(char* executable);
t_cmdlst		*parse_tokenlist(t_tokenlist *toklst);
void			print_cmdlst(t_cmdlst* cmd);
t_inputstream*	init_inputstream(char* input);
t_token*		get_next_token(t_inputstream* lexer);

t_cmdlst	*cmdlst_new(char *exec);
t_cmdlst	*cmdlst_last(t_cmdlst *head);
void		cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend);
int			cmdlst_size(t_cmdlst *lst);
void		cmdlst_clear(t_cmdlst **lst);


#endif

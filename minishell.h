/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/20 12:23:38 by fmaurer          ###   ########.fr       */
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
    TOK_CMD,	// executable or command
    TOK_ARG,	// command argument or file
    TOK_PIP,	// |
    TOK_ROUT,	// >
	TOK_OF,		// outfile after a >
	TOK_RIN,	// <
	TOK_IF,		// infile after a <
	TOK_AOUT,	// >>
	TOK_AIN, 	// <<
    TOK_EOF,  // end of input
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

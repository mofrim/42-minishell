/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/26 13:12:29 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* For ft_printf(), ft_strlen(), ... */
# include "libft/libft.h"

/* For signal */
# include <signal.h>

/* For FILE (needed by readline) */
# include <stdio.h>

/* For malloc, free */
# include <stdlib.h>

/* Well, for readline() */
# include <readline/readline.h>

/* For add_history() */
# include <readline/history.h>

# define PROMPT "$"

/* Define termios type for easier reference. */
typedef struct termios	t_termios;

/*********** Datatypes for tokenization. ***********/

/* The highest Token Number. Useful for iterating over Tokens, maybe?! */
# define TOKEN_MAX 19

/* Token types... are these really all? */
typedef enum e_toktype
{
	TOK_WORD		= 0,
	TOK_CMD			= 1,
	TOK_ARG			= 2,
	TOK_PIP			= 3,
	TOK_ROUT		= 4,
	TOK_RIN			= 5,
	TOK_OF			= 6,
	TOK_IF			= 7,
	TOK_ROUTA		= 8,
	TOK_HERE		= 9,
	TOK_HERE_DLIM	= 10,
	TOK_SQUOT		= 11,
	TOK_DQUOT		= 12,
	TOK_SQUOT_TXT	= 13,
	TOK_DQUOT_TXT	= 14,
	TOK_VAR_SYM		= 15,
	TOK_VAR_NAME	= 16,
	TOK_BLTIN		= 17,
	TOK_BLTIN_ARG	= 18,
	TOK_EOF			= 19
}	t_toktype;

/* Token structure */
typedef struct s_token
{
	t_toktype	type;
	char		*value;
}	t_token;

/* Inputstream structure. squot/dquot flags for quotation recognition during
 * tokenization. */
typedef struct s_cmdline
{
	char	*input;
	int		pos;
	int		length;
	int		squot_flag;
	int		dquot_flag;
	int		var_flag;
}	t_cmdline;

typedef struct s_tokenlist
{
	t_token				*token;
	struct s_tokenlist	*next;
}	t_tokenlist;

/*********** Datatypes for parsing. ***********/

/* Command structure for parsing. */
typedef struct s_cmdlst
{
	char			*executable;
	char			**args;
	int				arg_count;
	char			*output_file;
	char			*input_file;
	struct s_cmdlst	*next;
}	t_cmdlst;

/*********** Signal and terminal setup. ***********/

void		signal_handler(int signum);
void		signal_setup(void (*sig_handler)(int));
void		term_setup(struct termios *old_settings);

/*********** Utils ***********/

void		nullcheck(void *p, char *msg);
void		error_exit(char *msg);
int			ft_isspace(char c);

/*********** Tokenization. ***********/

t_tokenlist	*toklst_new(t_token *tok);
t_tokenlist	*toklst_last(t_tokenlist *head);
void		toklst_add_back(t_tokenlist **head, t_tokenlist *newend);
void		toklst_clear(t_tokenlist **lst);
int			toklst_size(t_tokenlist *lst);
t_tokenlist	*tokenize(char *input);
void		print_tokentype(t_token *token);

/*********** Parsing. ***********/

t_cmdlst	*new_command(char *executable);
t_cmdlst	*parse_tokenlist(t_tokenlist *toklst);
void		print_cmdlst(t_cmdlst *cmd);
t_cmdline	*init_cmdline(char *input);
t_token		*get_next_token(t_cmdline	*lexer);

t_cmdlst	*cmdlst_new(char *exec);
t_cmdlst	*cmdlst_last(t_cmdlst *head);
void		cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend);
int			cmdlst_size(t_cmdlst *lst);
void		cmdlst_clear(t_cmdlst **lst);

#endif

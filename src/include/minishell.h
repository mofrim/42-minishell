/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 10:48:35 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* For FILE (needed by readline) */
# include <stdio.h>

/* open and constants like O_RDONLY */
# include <fcntl.h>

/* pipe, dup, write */
# include <unistd.h>

/* For ft_printf(), ft_strlen(), ... */
# include "libft.h"

/* Well, for readline() */
# include "readline.h"

/* For add_history() */
# include "history.h"

/* For signal */
# include <signal.h>

/* For temios */
# include <termios.h>

/* For malloc, free */
# include <stdlib.h>

/* For waitpid */
# include <sys/wait.h>

/* Errors */
# include <errno.h>

/* COM = Colors over minishell! */
# include "colors.h"

# define PROMPT "\033[1;33m$\033[0m "

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

typedef enum e_tokerr
{
	TOKERR_PIP,
	TOKERR_NL,
	TOKERR_RIN,
	TOKERR_ROUT,
	TOKERR_ROUTA,
}	t_tokerr;

/*********** Datatypes for parsing. ***********/

/* Command structure for parsing. */
typedef struct s_cmdlst
{
	char			*cmd;
	char			**args;
	int				arg_count;
	int				is_builtin;
	int				heredoc;
	int				append;
	char			*output_file;
	char			*input_file;
	struct s_cmdlst	*next;
}	t_cmdlst;

/*********** Datatype for env. ***********/

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next ;
}	t_envlst;

/*********** Signal and terminal setup. ***********/

void		signal_handler(int signum);
void		signal_setup(void (*sig_handler)(int));
void		term_setup(struct termios *old_settings);

/*********** Utils ***********/

void		nullcheck(void *p, char *msg);
void		error_exit(char *msg);
int			ft_isspace(char c);
void		free_ptrptr(char ***ptr);

/*********** Tokenization. ***********/

t_tokenlist	*toklst_new(t_token *tok);
t_tokenlist	*toklst_last(t_tokenlist *head);
void		toklst_add_back(t_tokenlist **head, t_tokenlist *newend);
void		toklst_clear(t_tokenlist **lst);
int			toklst_size(t_tokenlist *lst);
void		print_tokentype(t_token *token);
void		print_toklst(t_tokenlist *tlst);
t_tokenlist	*tokenize(char *input);
t_tokenlist	*tokenize_lvl1(char *input);
int			tokenize_lvl2(t_tokenlist	*toklst);
int			check_toklst_lvl2(t_tokenlist *toklst);

/*********** Parsing. ***********/

t_cmdlst	*new_command(char *executable);
void		print_cmdlst(t_cmdlst *cmd);
t_cmdline	*init_cmdline(char *input);
t_token		*get_next_token(t_cmdline	*lexer);

t_cmdlst	*cmdlst_new(char *exec);
t_cmdlst	*cmdlst_last(t_cmdlst *head);
void		cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend);
void		cmdlst_clear(t_cmdlst **lst);

t_cmdlst	*parse_tokenlist(t_tokenlist *toklst);
void		parse_command(t_tokenlist **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd);
void		parse_builtin(t_tokenlist **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd);
void		parse_pipe(t_tokenlist **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd);
void		parse_rout(t_tokenlist **toklst, t_cmdlst *cur_cmd);
void		parse_rin(t_tokenlist **toklst, t_cmdlst *cur_cmd);

/*********** Env. ***********/
void		print_env(char **env);
t_envlst	*parse_env(char **env);
void		print_envlst(t_envlst *el);
t_envlst	*envlst_new(char *name, char *value);
t_envlst	*envlst_last(t_envlst *head);
void		envlst_add_back(t_envlst **head, t_envlst *newend);
int			envlst_size(t_envlst *lst);
void		envlst_clear(t_envlst **lst);
char		**get_env_array(t_envlst *el);
char		*get_env_value(char *name, t_envlst *el);
t_envlst	*get_env_entry_by_name(char *name, t_envlst *el);
void 		set_env_entry(char *name, char *value, t_envlst **el);

/*********** Exec. ***********/
char	*get_exec_path(t_cmdlst *clst, char **env);
int		exec_cmd(t_cmdlst *cmdl, t_envlst *el);
int		exec_complex_cmd(t_cmdlst *cmdl, char **env);

#endif

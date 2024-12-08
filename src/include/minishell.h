/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/08 18:56:41 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* For FILE (needed by readline), perror */
# include <stdio.h>

/* open and constants like O_RDONLY */
# include <fcntl.h>

/* pipe, dup, write, waitpid, execve */
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

# define PROMPT "\033[1;33m=8-)\033[0m "

/* Define termios type for easier reference. */
typedef struct termios	t_termios;

/*********** Datatype for env. ***********/

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next ;
}	t_envlst;

/*********** Datatypes for tokenization. ***********/

/* The highest Token Number. Useful for iterating over Tokens, maybe?! */
# define TOKEN_MAX 17

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
	TOK_VAR_SYM		= 13,
	TOK_VAR_NAME	= 14,
	TOK_VAR_QUOT	= 15,
	TOK_BLTIN		= 16,
	TOK_EOF			= 17
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
	char		*input;
	int			pos;
	int			length;
	int			squot_flag;
	int			dquot_flag;
	int			var_flag;
	t_envlst	*env;
}	t_cmdline;

typedef struct s_toklst
{
	t_token			*token;
	struct s_toklst	*next;
}	t_toklst;

typedef enum e_tokerr
{
	TOKERR_PIP,
	TOKERR_NL,
	TOKERR_RIN,
	TOKERR_ROUT,
	TOKERR_ROUTA,
	TOKERR_HERE,
}	t_tokerr;

/*********** Datatypes for parsing. ***********/

/* Command structure for parsing. */
typedef struct s_cmdlst
{
	char			*cmd;
	char			**args;
	int				arg_count;
	int				is_builtin;
	char			*heredoc;
	int				append;
	char			*output_file;
	char			*input_file;
	struct s_cmdlst	*next;
}	t_cmdlst;

/* Struct for recording the heredoc delimiters. */
typedef struct s_heredoc
{
	char				*delim;
	struct	s_heredoc	*next;
}	t_heredoc;

/*********** Signal and terminal setup. ***********/

void		signal_handler(int signum);
void		signal_setup(void (*sig_handler)(int));
void		term_setup(struct termios *old_settings);

/*********** Utils ***********/

void		nullcheck(void *p, char *msg);
void		error_exit(char *msg);
int			ft_isspace(char c);
void		free_ptrptr(char ***ptr);
int			print_return_error_msg(char *prefix, char *msg, int error);

/*********** Tokenization. ***********/

t_toklst	*toklst_new(t_token *tok);
t_toklst	*toklst_last(t_toklst *head);
void		toklst_add_back(t_toklst **head, t_toklst *newend);
void		toklst_clear(t_toklst **lst);
int			toklst_size(t_toklst *lst);
void		toklst_del(t_toklst **lst, t_toklst *delme);
void		toklst_remove_tok(t_toklst **toklst, t_toklst **tl);
void		print_tokentype(t_token *token);
void		print_toklst(t_toklst *tlst);
t_toklst	*tokenize(char *input, t_envlst *env);
t_toklst	*tokenize_lvl1(char *input, t_envlst *env);
int			tokenize_lvl2(t_toklst	**toklst);
int			check_toklst_lvl2(t_toklst *toklst);

/*********** Parsing. ***********/

t_cmdlst	*new_command(char *executable);
void		print_cmdlst(t_cmdlst *cmd);
t_cmdline	*init_cmdline(char *input, t_envlst *env);
t_token		*get_next_token(t_cmdline *cl);

t_cmdlst	*cmdlst_new(char *exec, int maxargs);
t_cmdlst	*cmdlst_last(t_cmdlst *head);
void		cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend);
void		cmdlst_clear(t_cmdlst **lst);

t_cmdlst	*parse_tokenlist(t_toklst *toklst);
void		parse_command(t_toklst **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd, int maxargs);
void		parse_builtin(t_toklst **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd, int maxargs);
void		parse_pipe(t_toklst **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd, int maxargs);
void		parse_rout(t_toklst **toklst, t_cmdlst *cur_cmd);
void		parse_rin(t_toklst **toklst, t_cmdlst *cur_cmd);
void		parse_heredoc(t_toklst **tlst, t_cmdlst *cur_cmd);

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
void		set_env_entry(char *name, char *value, t_envlst **el);

/*********** Exec. ***********/
char		*get_exec_path(t_cmdlst *clst, char **env);
int			exec_cmd(t_cmdlst *cmdl, t_envlst **el);
int			exec_redir_cmd(t_cmdlst *cmdl, char **env);
int			exec_single(t_cmdlst *cmdl, char **env, t_envlst **el);
int			exec_single_redir_cmd(t_cmdlst *cmdl, char **env);
int			exec_single_builtin_cmd(t_cmdlst *cmdl, t_envlst **el);
int			exec_pipe(t_cmdlst *cmdl, char **env, t_envlst **el);
int			open_redir_files(char *infile, char *outfile, int append);

/*********** Builtins. ***********/
int			bltin_echo(char **arg, t_envlst **el);
int			bltin_cd(char **arg, t_envlst **el);
int			bltin_pwd(char **arg, t_envlst **el);
int			bltin_export(char **arg, t_envlst **el);
int			bltin_unset(char **arg, t_envlst **el);
int			bltin_env(char **arg, t_envlst **el);
int			bltin_exit(void);

#endif

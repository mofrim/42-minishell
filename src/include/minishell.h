/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/02 20:10:27 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* For FILE (needed by readline), perror */
# include <stdio.h>

/* ioctl */
# include <sys/ioctl.h>

/* For struct stat, stat() and S_IFREG. */
# include <sys/stat.h>

/* open and constants like O_RDONLY */
# include <fcntl.h>

/* pipe, dup, write, waitpid, execve, STDIN_FILENO, _POSIX_VDISABLE */
# include <unistd.h>

/* strerror */
# include <string.h>

/* For ft_printf(), ft_strlen(), ... */
# include "libft.h"

/* Well, for readline() */
# include "readline.h"

/* For add_history() */
# include "history.h"

/* For signal, kill */
# include <signal.h>

/* For t_termios, tcsetattr, tcgetattr, ISIG, VQUIT, ... */
# include <termios.h>

/* For malloc, free */
# include <stdlib.h>

/* For waitpid */
# include <sys/wait.h>

/* Errors */
# include <errno.h>

/* COM = Colors over minishell! */
# include "colors.h"

/* The \001 and \002 esc seqs tell readline where non-printable characters 
 * start / end in the prompt. Fixes outputs problems after long text inputs. */
# define PROMPT "\001\033[1;33m\002=8-)\001\033[0m\002 "

/* Define termios type for easier reference. */
typedef struct termios	t_termios;

/*********** Datatype for env. ***********/

typedef struct s_envlst
{
	char			*name;
	char			*value;
	struct s_envlst	*next ;
	struct s_envlst	*prev ;
}	t_envlst;

/*********** Datatypes for tokenization. ***********/

/* Token types... are these really all? 
 *
 * TOK_ROUT0:	simple '> word'
 * TOK_ROUT1:	'[n]> word'. word is always TOK_OF! no fildes num exansion!
 * TOK_ROUT2:	'&>word'. this is the only legit sequence involving &>! in
 * 				'2&>1' the '2' is counted as an arg and the outfile will be 
 * 				called '1'! redir stdout & stderr to word.
 * TOK_ROUT3:	'[n]>&word'
 * TOK_QCMD:	quoted cmd means: don't split me in tok lvl3!
 *
 * */
typedef enum e_toktype
{
	TOK_NULL,
	TOK_WHITE,
	TOK_WORD,
	TOK_QWORD,
	TOK_VWORD,
	TOK_CMD,
	TOK_QCMD,
	TOK_ARG,
	TOK_PIP,
	TOK_ROUT0,
	TOK_ROUT1,
	TOK_ROUT2,
	TOK_ROUT3,
	TOK_ROUT_FDFROM,
	TOK_ROUT3_FDFROM,
	TOK_ROUT3_FDTO,
	TOK_ROUTA0,
	TOK_ROUTA1,
	TOK_ROUTA2,
	TOK_ROUTA_FDFROM,
	TOK_ROUTA_FDTO,
	TOK_RIN0,
	TOK_RIN1,
	TOK_RIN_FDTO,
	TOK_RINOUT0,
	TOK_RINOUT1,
	TOK_OF,
	TOK_IF,
	TOK_IFOF,
	TOK_IFOF_FD,
	TOK_AND,
	TOK_HERE,
	TOK_HERE_DLIM,
	TOK_HERE_QDLIM,
	TOK_SQUOT,
	TOK_DQUOT,
	TOK_VAR_SYM,
	TOK_VAR_NAME,
	TOK_VAR_QUOT,
	TOK_BLTIN
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
	int			herdlim_flag;
	int			var_flag;
	t_envlst	*env;
}	t_cmdline;

typedef struct s_toklst
{
	t_token			*token;
	struct s_toklst	*next;
	struct s_toklst	*prev;
}	t_toklst;

typedef enum e_tokerr
{
	TOKERR_PIP,
	TOKERR_NL,
	TOKERR_RIN,
	TOKERR_ROUT,
	TOKERR_ROUT2,
	TOKERR_ROUT3,
	TOKERR_ROUTA,
	TOKERR_ROUTA2,
	TOKERR_REDIR,
	TOKERR_HERE,
	TOKERR_AND,
	TOKERR_FDFROM,
	TOKERR_RINOUT
}	t_tokerr;

/* Error msg used in print_tokerr_int */
# define ERRMSG_SYNTAX "minishell: Syntax error near unexpected token: "

/*********** Datatypes for parsing. ***********/

/* Enum with all possible redirtypes we support. */
typedef enum e_redirtype
{
	RE_RIN0,
	RE_RIN1,
	RE_RINOUT,
	RE_ROUT0,
	RE_ROUT1,
	RE_ROUT2,
	RE_ROUT3,
	RE_ROUTA0,
	RE_ROUTA1,
	RE_ROUTA2,
	RE_DOC,
	RE_QDOC
}	t_redirtype;

/* Struct for recording heredoc delimiters. */
typedef struct s_herdlst
{
	char				*name;
	t_toktype			type;
	struct s_herdlst	*next;
}	t_herdlst;

/* Struct for saving all redirects. */
typedef struct s_redirlst
{
	t_redirtype			redtype;
	int					fd_from;
	int					fd_to;
	char				*outfile;
	char				*infile;
	struct s_redirlst	*next;
}	t_redirlst;

/* Command structure for parsing. */
typedef struct s_cmdlst
{
	char			*cmd;
	char			**args;
	int				arg_count;
	int				cmd_count;
	int				is_builtin;
	int				exit_flag;
	t_herdlst		*heredocs;
	int				preout_flag;
	t_redirlst		*redirs;
	struct s_cmdlst	*next;
	struct s_cmdlst	*prev;
}	t_cmdlst;

/*********** The ministruct. ***********/

/* Convenience struct for initialization parameter handling. */
typedef struct s_ministruct
{
	t_toklst	*tl;
	t_envlst	*el;
	t_termios	*term;
	char		**envp;
	int			ac;
	char		**av;
	int			script_mode;
	int			stdin_istty;
}	t_ministruct;

/*********** Exec bltin_pipe struct. ***********/

typedef struct s_bltin_pipargs
{
	t_cmdlst	*cl;
	t_envlst	**el;
	int			*prev_read;
}	t_bltin_pipargs;

/*********** Heredoc tmpfile struct. ***********/

typedef struct s_htmpfile
{
	char	*filename;
	int		fd;
}	t_htmpfile;

/*********** Signal and terminal setup. ***********/

void		minish_sighandler(int signum);
int			signal_setup(void (*sig_handler)(int));
void		term_setup(struct termios *old_settings);
int			ft_wifsignaled(int status);
int			ft_wexitstatus(int status);
void		heredoc_sig_handler(int signum);
void		init_shell_vars(t_envlst **el);

/*********** Utils ***********/

void		nullcheck(void *p, char *msg);
void		error_exit(char *msg);
int			ft_isspace(char c);
void		free_ptr2ptr(char ***ptr);
int			minish_errormsg(char *prefix, char *msg, int error);
int			minish_errormsg2(char *prefix, char *arg, char *msg, int error);
int			ft_isnum(char c);
int			get_posint_numstr(char *s);
int			splitsize(char **s);
void		read_prompt(char **input, char *prompt);

/*********** Tokenization. ***********/

t_token		*init_token(char *value, int type);
t_toklst	*toklst_new(t_token *tok);
t_toklst	*toklst_new_tok(t_toktype type, char *value);
t_toklst	*toklst_last(t_toklst *head);
void		toklst_add_back(t_toklst **head, t_toklst *newend);
void		toklst_clear(t_toklst **lst);
int			toklst_size(t_toklst *lst);
void		free_one_toklst(t_toklst *lst);
void		toklst_del(t_toklst **lst, t_toklst *delme);
void		toklst_remove_tok(t_toklst **toklst, t_toklst **tl);
void		print_tokentype(t_token *token);
void		print_toklst(t_toklst *tlst);

t_toklst	*tokenize(char *input, t_envlst **el);
t_toklst	*tokenize_lvl1(char *input, t_envlst *env);
int			tokenize_lvl2(t_toklst	**toklst);
int			tokenize_lvl3(t_toklst	**toklst);
int			lvl2_check_toklst(t_toklst *toklst);
void		apply_redir_tokenization(t_token *prev, t_token *cur, \
		t_token *next);

int			print_tokerr(t_tokerr te, char *tok);
t_toktype	is_cmd_or_builtin(char *val);
int			is_rin_tok(t_toktype tok);
int			is_rout_tok(t_toktype tok);
int			is_redir_tok(t_toktype tok);
int			is_word_tok(t_toktype tt);
int			is_valid_varname_char(char next);
void		split_tokens_with_whitespaces(t_toklst **tlst);
t_toklst	*split_vword_token(char *vword);
int			has_whitespace(char *str);

/*********** Parsing. ***********/

t_cmdlst	*new_command(char *executable);
void		print_cmdlst(t_cmdlst *cmd);
t_cmdline	*init_cmdline(char *input, t_envlst *env);
t_token		*get_next_token(t_cmdline *cl);

t_cmdlst	*cmdlst_new(char *exec, int maxargs);
t_cmdlst	*cmdlst_last(t_cmdlst *head);
t_cmdlst	*cmdlst_head(t_cmdlst *cur);
void		cmdlst_add_back(t_cmdlst **head, t_cmdlst *newend);
void		cmdlst_clear(t_cmdlst **lst);

t_herdlst	*herdlst_new(t_token *tok);
t_herdlst	*herdlst_last(t_herdlst *head);
void		herdlst_add_back(t_herdlst **head, t_herdlst *newend);
void		herdlst_clear(t_herdlst **lst);
void		herdlst_print(t_herdlst *lst);

t_redirlst	*redirlst_new(t_redirlst new);
t_redirlst	*redirlst_last(t_redirlst *head);
void		redirlst_add_back(t_redirlst **head, t_redirlst *newend);
void		redirlst_clear(t_redirlst **lst);
void		redirlst_print(t_redirlst *lst);
void		init_redirlst_var(t_redirlst *var);

t_cmdlst	*parse_tokenlist(t_toklst **toklst);
void		parse_command(t_toklst **toklst, t_cmdlst **cmd, \
		t_cmdlst **cur_cmd, int maxargs);
void		parse_args(t_toklst **toklst, t_cmdlst **cur_cmd);
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
t_envlst	*envlst_copy(t_envlst *env);
int			envlst_delone_by_name(char *name, t_envlst **el);

/*********** Exec. ***********/

char		*get_exec_path(t_cmdlst *clst, char **env);
int			exec_cmd(t_cmdlst *cmdl, t_envlst **el);
int			exec_redir_cmd(t_cmdlst *cmdl, char **env);
int			exec_single(t_cmdlst *cmdl, char **env, t_envlst **el);
int			exec_single_builtin_cmd(t_cmdlst *cl, t_envlst **el);
int			exec_pipeline(t_cmdlst *cmdl, char **env, t_envlst **el);
int			open_redir_files(t_redirlst *rdl);
int			set_exec_path(t_cmdlst *cl, char **env);

/*********** The HEREDOC. ***********/

int			heredoc(t_cmdlst *cl, t_ministruct *mini);
void		heredoc_cleanup(t_cmdlst *cl);

/*********** Builtins. ***********/

int			bltin_export_preout(t_cmdlst *cl, t_envlst **el);
int			bltin_export_out(t_cmdlst *cl, t_envlst **el);

int			bltin_exit_preout(t_cmdlst *cl, t_envlst **el);
int			bltin_exit_out(t_cmdlst *cl, t_envlst **el);

int			bltin_cd_preout(t_cmdlst *cl, t_envlst **el);
int			bltin_cd_out(t_cmdlst *cl, t_envlst **el);

int			bltin_env(t_cmdlst *cl, t_envlst **el);
int			bltin_echo(t_cmdlst *cl, t_envlst **el);
int			bltin_pwd(t_cmdlst *cl, t_envlst **el);
int			bltin_unset(t_cmdlst *cl, t_envlst **el);

#endif

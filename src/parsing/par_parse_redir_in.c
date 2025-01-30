/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_redir_in.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 16:00:04 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/30 19:54:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_rin0(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);
static void	parse_rin1(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);
static void	parse_rinout(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);

/* Parse a input redirect. If there was already an input_file set: free it an
 * replace with new one found. Through this we immitate bash's behavior that
 * always the last redirect for the same fildes wins. */
void	parse_rin(t_toklst **toklst, t_cmdlst *cur_cmd)
{
	t_toktype	ttype;

	if (*toklst)
		ttype = (*toklst)->token->type;
	if (*toklst && (ttype == TOK_RIN0))
		parse_rin0(toklst, cur_cmd, ttype);
	if (*toklst && (ttype == TOK_RIN_FDTO))
		parse_rin1(toklst, cur_cmd, ttype);
	if (*toklst && (ttype == TOK_RINOUT0 || ttype == TOK_IFOF_FD))
		parse_rinout(toklst, cur_cmd, ttype);
}

/* Parse `< file` tokens. */
static void	parse_rin0(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	(void)ttype;
	init_redirlst_var(&rfl);
	rfl.redtype = RE_RIN0;
	rfl.infile = (*toklst)->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	*toklst = (*toklst)->next->next;
}

/* Parse `n<file` tokens. */
void	parse_rin1(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	(void)ttype;
	init_redirlst_var(&rfl);
	rfl.redtype = RE_RIN1;
	rfl.fd_to = ft_atoi((*toklst)->token->value);
	rfl.infile = (*toklst)->next->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	*toklst = (*toklst)->next->next->next;
}

/* Parse `<>` tokens. */
void	parse_rinout(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	init_redirlst_var(&rfl);
	rfl.redtype = RE_RINOUT;
	if (ttype == TOK_IFOF_FD)
	{
		rfl.fd_to = ft_atoi((*toklst)->token->value);
		rfl.infile = (*toklst)->next->next->token->value;
	}
	else
		rfl.infile = (*toklst)->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	if (ttype == TOK_IFOF_FD)
		*toklst = (*toklst)->next->next->next;
	else
		*toklst = (*toklst)->next->next;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parse_redir_out.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 21:23:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/27 16:08:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_rout0(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);
static void	parse_rout1(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);
static void	parse_rout2(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);
static void	parse_rout3(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype);

void	parse_rout(t_toklst **toklst, t_cmdlst *cur_cmd)
{
	t_toktype	ttype;

	if (*toklst)
		ttype = (*toklst)->token->type;
	if (*toklst && (ttype == TOK_ROUT0 || ttype == TOK_ROUTA0))
		parse_rout0(toklst, cur_cmd, ttype);
	if (*toklst && (ttype == TOK_ROUT_FDFROM || ttype == TOK_ROUTA_FDFROM))
		parse_rout1(toklst, cur_cmd, ttype);
	if (*toklst && (ttype == TOK_ROUT2 || ttype == TOK_ROUTA2))
		parse_rout2(toklst, cur_cmd, ttype);
	if (*toklst && (ttype == TOK_ROUT3 || ttype == TOK_ROUT3_FDFROM))
		parse_rout3(toklst, cur_cmd, ttype);
}

void	parse_rout0(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	init_redirlst_var(&rfl);
	if (ttype == TOK_ROUTA0)
		rfl.redtype = RE_ROUTA0;
	else
		rfl.redtype = RE_ROUT0;
	rfl.outfile = (*toklst)->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	*toklst = (*toklst)->next->next;
}

void	parse_rout1(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	init_redirlst_var(&rfl);
	if (ttype == TOK_ROUTA_FDFROM)
		rfl.redtype = RE_ROUTA1;
	else
		rfl.redtype = RE_ROUT1;
	rfl.fd_from = ft_atoi((*toklst)->token->value);
	rfl.outfile = (*toklst)->next->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	*toklst = (*toklst)->next->next->next;
}

void	parse_rout2(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	init_redirlst_var(&rfl);
	if (ttype == TOK_ROUTA2)
		rfl.redtype = RE_ROUTA2;
	else
		rfl.redtype = RE_ROUT2;
	rfl.outfile = (*toklst)->next->token->value;
	redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
	*toklst = (*toklst)->next->next;
}

void	parse_rout3(t_toklst **toklst, t_cmdlst *cur_cmd, t_toktype ttype)
{
	t_redirlst	rfl;

	init_redirlst_var(&rfl);
	rfl.redtype = RE_ROUT3;
	if (ttype == TOK_ROUT3_FDFROM)
	{
		rfl.fd_from = get_posint_numstr((*toklst)->token->value);
		rfl.fd_to = get_posint_numstr((*toklst)->next->next->token->value);
		redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
		*toklst = (*toklst)->next->next->next;
	}
	else if (ttype == TOK_ROUT3)
	{
		rfl.fd_to = get_posint_numstr((*toklst)->next->token->value);
		redirlst_add_back(&cur_cmd->redirs, redirlst_new(rfl));
		*toklst = (*toklst)->next->next;
	}
}

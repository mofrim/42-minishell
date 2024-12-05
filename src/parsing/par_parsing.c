/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 10:49:43 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Intermediate helper function for skipping forward toklst. When
 * parse_tokenlist() finally suppports all possible tokens, this will be
 * deprecated. */
static void	fwdlst(t_tokenlist **tl)
{
	t_toktype	tok;

	if (!*tl)
		return ;
	tok = (*tl)->token->type;
	if ((tok != TOK_RIN && tok != TOK_CMD && tok != TOK_ARG && \
				tok != TOK_ROUTA && tok != TOK_ROUT && tok != TOK_PIP && \
				tok != TOK_IF && tok != TOK_OF && tok != TOK_BLTIN) || \
			((tok != TOK_CMD || tok != TOK_BLTIN) && (*tl)->next == NULL))
		(*tl) = (*tl)->next;
}

/* Parse tokenlist into cmdlist. */
t_cmdlst	*parse_tokenlist(t_tokenlist *toklst)
{
	t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;

	if (!toklst)
		return (NULL);
	cmd = cmdlst_new(NULL);
	cur_cmd = cmd;
	while (toklst)
	{
		parse_command(&toklst, &cmd, &cur_cmd);
		parse_builtin(&toklst, &cmd, &cur_cmd);
		parse_pipe(&toklst, &cmd, &cur_cmd);
		parse_rout(&toklst, cur_cmd);
		parse_rin(&toklst, cur_cmd);
		fwdlst(&toklst);
	}
	return (cmd);
}

/* Print command structure (for debugging) */
void	print_cmdlst(t_cmdlst *cmd)
{
	int	i;

	while (cmd)
	{
		ft_printf("-- cmd start --\n");
		ft_printf("Command: %s\n", cmd->cmd);
		ft_printf("Arguments: ");
		i = 0;
		while (++i < cmd->arg_count)
			ft_printf("%s ", cmd->args[i]);
		ft_printf("\n");
		if (cmd->output_file)
			ft_printf("Output redirected to: %s\n", cmd->output_file);
		if (cmd->input_file)
			ft_printf("Input redirected from: %s\n", cmd->input_file);
		if (cmd->is_builtin)
			ft_printf("Is builtin!\n");
		if (cmd->heredoc)
			ft_printf("Has HEREDOC!\n");
		if (cmd->next)
			ft_printf("Piped to: %s\n", cmd->next->cmd);
		ft_printf("-- cmd end --\n");
		cmd = cmd->next;
	}
}

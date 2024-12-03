/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/03 12:10:50 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse tokenlist into cmdlist. */
t_cmdlst	*parse_tokenlist(t_tokenlist *toklst)
{
	t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;

	cmd = NULL;
	while (toklst)
	{
		if (!cmd)
		{
			cmd = cmdlst_new(NULL);
			cur_cmd = cmd;
		}
		parse_command(&toklst, &cmd, &cur_cmd);
		parse_pipe(&toklst, &cmd, &cur_cmd);
		parse_rout(&toklst, cur_cmd);
		if (toklst && toklst->next && toklst->token->type == TOK_RIN)
		{
			cur_cmd->input_file = ft_strdup(toklst->next->token->value);
			toklst = toklst->next->next;
		}
		else if (toklst)
			toklst = toklst->next;
	}
	return (cmd);
}

/* Print command structure (for debugging) */
void	print_cmdlst(t_cmdlst *cmd)
{
	int	i;

	while (cmd)
	{
		ft_printf("Command: %s\n", cmd->executable);
		ft_printf("Arguments: ");
		i = -1;
		while (++i < cmd->arg_count)
			printf("%s ", cmd->args[i]);
		printf("\n");
		if (cmd->output_file)
			ft_printf("Output redirected to: %s\n", cmd->output_file);
		if (cmd->input_file)
			ft_printf("Input redirected from: %s\n", cmd->input_file);
		if (cmd->is_builtin)
			ft_printf("Is builtin!\n");
		if (cmd->heredoc)
			ft_printf("Has HEREDOC!\n");
		if (cmd->next)
			ft_printf("Piped to: %s\n", cmd->next->executable);
		ft_printf("--\n");
		cmd = cmd->next;
	}
}

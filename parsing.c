/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/20 00:26:21 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "minishell.h"

/* Parse tokenlist into cmdlist. */
t_cmdlst	*parse_tokenlist(t_tokenlist *toklst)
{
    t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;

	cmd = NULL;
    while (toklst)
	{
		if (toklst->token->type == TOKEN_COMMAND) 
		{
			if (!cmd)
			{
				cmd = cmdlst_new(toklst->token->value);
				cur_cmd = cmd;
			}
			else
			{
				cur_cmd = cmdlst_new(toklst->token->value);
				cmdlst_add_back(&cmd, cur_cmd);
			}
			if (toklst->next == NULL)
				return (cmd);
			toklst = toklst->next;
			while (toklst->token->type == TOKEN_ARG) 
			{
				cur_cmd->args[cur_cmd->arg_count++] = ft_strdup(toklst->token->value);
				if (toklst->next == NULL)
					return (cmd);
				toklst = toklst->next;
			}
		}
		// FIXME this is not safe from syntactic errors, f.ex. "ls -al | | | >"
		else if (toklst->token->type == TOKEN_PIPE && \
				toklst->next->token->type == TOKEN_COMMAND)
		{
			toklst = toklst->next;
			// cmdlst_add_back(&cmd, cmdlst_new(toklst->token->value));
		} 
		// FIXME this is not safe from syntactic errors
		else if (toklst->token->type == TOKEN_REDIRECT && \
				toklst->next->token->type == TOKEN_ARG)
		{
			toklst = toklst->next;
			cur_cmd->output_file = ft_strdup(toklst->token->value);
		}
	}
    return (cmd);
}

/* Print command structure (for debugging) */
void	print_cmdlst(t_cmdlst* cmd) {
	while (cmd) {
		printf("Command: %s\n", cmd->executable);
		printf("Arguments: ");
		for (int i = 0; i < cmd->arg_count; i++) {
			printf("%s ", cmd->args[i]);
		}
		printf("\n");
		if (cmd->output_file) {
			printf("Output redirected to: %s\n", cmd->output_file);
		}
		if (cmd->next) {
			printf("Piped to: %s\n", cmd->next->executable);
		}
		ft_printf("--\n");
		cmd = cmd->next;
	}
}

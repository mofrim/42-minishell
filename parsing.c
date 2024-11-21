/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:59:44 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/21 00:12:07 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "minishell.h"

static void	parse_command(t_tokenlist **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
{
	if (*cur_cmd && (*cur_cmd)->executable)
	{
		*cur_cmd = cmdlst_new((*toklst)->token->value);
		cmdlst_add_back(cmd, *cur_cmd);
	}
	else
		(*cur_cmd)->executable = ft_strdup((*toklst)->token->value);
	if ((*toklst)->next == NULL)
		return ;
	*toklst = (*toklst)->next;
	while ((*toklst)->token->type == TOK_ARG) 
	{
		(*cur_cmd)->args[(*cur_cmd)->arg_count++] = ft_strdup((*toklst)->token->value);
		if ((*toklst)->next == NULL)
			return ;
		*toklst = (*toklst)->next;
	}
}

static int	parse_pipe(t_tokenlist **toklst, t_cmdlst **cmd, t_cmdlst **cur_cmd)
{
	if ((*toklst)->token->type == TOK_PIP && \
			(*toklst)->next == NULL)
		return (-1);
	if ((*toklst)->token->type == TOK_PIP && \
			(*toklst)->next->token->type == TOK_PIP)
		return (-1);
	if ((*toklst)->token->type == TOK_PIP && \
			(*toklst)->next->token->type == TOK_CMD)
	{
		*toklst = (*toklst)->next;
		return (1);
	}
	if ((*toklst)->token->type == TOK_PIP && \
			((*toklst)->next->token->type == TOK_RIN || \
			(*toklst)->next->token->type == TOK_ROUT))
	{
		*cur_cmd = cmdlst_new(NULL);
		cmdlst_add_back(cmd, *cur_cmd);
		*toklst = (*toklst)->next;
		return (1);
	}
	return (0);
}

/* Parse tokenlist into cmdlist. */
// TODO this has to be more elegant and shorter in order to obey the norm.
// because there are lot more tokens to be handled.
// IDEA Mayyyyybeee check syntax in advance!
t_cmdlst	*parse_tokenlist(t_tokenlist *toklst)
{
    t_cmdlst	*cmd;
	t_cmdlst	*cur_cmd;

	cmd = NULL;
    while (toklst)
	{
		// if we are at the very first command token-gathering init cmdlst with
		// NULL as exec -> need to fill that later
		if (!cmd)
		{
			cmd = cmdlst_new(NULL);
			cur_cmd = cmd;
		}
		if (toklst->token->type == TOK_CMD) 
		{
			parse_command(&toklst, &cmd, &cur_cmd);
			if (toklst->next == NULL)
				return (cmd);
		}
		else if (parse_pipe(&toklst, &cmd, &cur_cmd) == -1)
		{
			ft_printf("invalid pipe syntax!\n");
			return (NULL);
		}
		else if (toklst->token->type == TOK_ROUT && \
				toklst->next->token->type == TOK_OF)
		{
			cur_cmd->output_file = ft_strdup(toklst->next->token->value);
			toklst = toklst->next->next;
		}
		else if (toklst->token->type == TOK_RIN && \
				toklst->next->token->type == TOK_IF)
		{
			cur_cmd->input_file = ft_strdup(toklst->next->token->value);
			toklst = toklst->next->next;
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
		if (cmd->input_file) {
			printf("Input redirected to: %s\n", cmd->input_file);
		}
		if (cmd->next) {
			printf("Piped to: %s\n", cmd->next->executable);
		}
		ft_printf("--\n");
		cmd = cmd->next;
	}
}

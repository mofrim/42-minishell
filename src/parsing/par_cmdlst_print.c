/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_cmdlst_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:56:12 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/07 19:10:27 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_banner(t_cmdlst *cl);
static void	print_cmd(t_cmdlst *cl);

/* Print command structure (for debugging) */
void	print_cmdlst(t_cmdlst *cl)
{
	print_banner(cl);
	while (cl)
	{
		print_cmd(cl);
		cl = cl->next;
	}
	ft_printf(GRN "-- cmdlst end --\n\n" RST);
}

static void	print_banner(t_cmdlst *cl)
{
	ft_printf(GRN "-- cmdlst start --\n" RST, cl->cmd);
	ft_printf(GRN "number of cmds: %d\n" RST, cl->cmd_count);
}

static void	print_cmd(t_cmdlst *cl)
{
	int	i;

	ft_printf(GRN "-- cmd --\n" RST "Command: %s\nArguments: ", cl->cmd);
	i = 0;
	while (++i < cl->arg_count)
		ft_printf("%s ", cl->args[i]);
	ft_printf("\n");
	if (cl->redirs)
	{
		ft_printf("Redirects: ");
		redirlst_print(cl->redirs);
	}
	if (cl->is_builtin)
		ft_printf("Is builtin!\n");
	if (cl->heredocs)
	{
		ft_printf("heredocs: ");
		herdlst_print(cl->heredocs);
		ft_printf("\n");
	}
	if (cl->next)
		ft_printf("Piped to: %s\n", cl->next->cmd);
}

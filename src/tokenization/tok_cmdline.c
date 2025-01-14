/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_cmdline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:42:09 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/14 12:33:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize tokenizer with input string. Skip any whitespace chars at the
 * begin. */
t_cmdline	*init_cmdline(char *input, t_envlst *env)
{
	t_cmdline	*cl;

	cl = malloc(sizeof(t_cmdline));
	nullcheck(cl, "init_cmdline");
	cl->input = input;
	cl->pos = 0;
	cl->length = ft_strlen(input);
	cl->dquot_flag = 0;
	cl->squot_flag = 0;
	cl->var_flag = 0;
	cl->herdlim_flag = 0;
	cl->env = env;
	return (cl);
}

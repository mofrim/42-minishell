/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokprint.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 21:31:43 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/11 23:44:38 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_tokentype1(t_token *token)
{
	int	ret;

	ret = 0;
	if (token->type == TOK_CMD)
		ret = ft_printf("TOK_CMD");
	else if (token->type == TOK_ARG)
		ret = ft_printf("TOK_ARG");
	else if (token->type == TOK_DQUOT)
		ret = ft_printf("TOK_DQUOT");
	else if (token->type == TOK_VAR_SYM)
		ret = ft_printf("TOK_VAR_SYM");
	else if (token->type == TOK_VAR_NAME)
		ret = ft_printf("TOK_VAR_NAME");
	else if (token->type == TOK_BLTIN)
		ret = ft_printf("TOK_BLTIN");
	else if (token->type == TOK_WORD)
		ret = ft_printf("TOK_WORD");
	else if (token->type == TOK_PIP)
		ret = ft_printf("TOK_PIP");
	else if (token->type == TOK_HERE)
		ret = ft_printf("TOK_HERE");
	else if (token->type == TOK_HERE_DLIM)
		ret = ft_printf("TOK_HERE_DLIM");
	return (ret);
}

static int	print_tokentype2(t_token *token)
{
	int	ret;

	ret = 0;
	if (token->type == TOK_SQUOT)
		ret = ft_printf("TOK_SQUOT");
	else if (token->type == TOK_RIN0)
		ret = ft_printf("TOK_RIN0");
	else if (token->type == TOK_OF)
		ret = ft_printf("TOK_OF");
	else if (token->type == TOK_IF)
		ret = ft_printf("TOK_IF");
	else if (token->type == TOK_ROUT0)
		ret = ft_printf("TOK_ROUT");
	else if (token->type == TOK_ROUT1)
		ret = ft_printf("TOK_ROUT1");
	else if (token->type == TOK_ROUT2)
		ret = ft_printf("TOK_ROUT2");
	else if (token->type == TOK_ROUT3)
		ret = ft_printf("TOK_ROUT3");
	else if (token->type == TOK_ROUT_FDFROM)
		ret = ft_printf("TOK_ROUT_FDFROM");
	else if (token->type == TOK_ROUT3_FDTO)
		ret = ft_printf("TOK_ROUT3_FDTO");
	return (ret);
}

static int	print_tokentype3(t_token *token)
{
	int	ret;

	ret = 0;
	if (token->type == TOK_ROUTA0)
		ret = ft_printf("TOK_ROUTA0");
	else if (token->type == TOK_ROUTA1)
		ret = ft_printf("TOK_ROUTA1");
	else if (token->type == TOK_ROUTA2)
		ret = ft_printf("TOK_ROUTA2");
	else if (token->type == TOK_ROUTA_FDFROM)
		ret = ft_printf("TOK_ROUTA_FDFROM");
	else if (token->type == TOK_ROUT3_FDFROM)
		ret = ft_printf("TOK_ROUT3_FDFROM");
	else if (token->type == TOK_ROUTA_FDTO)
		ret = ft_printf("TOK_ROUTA_FDTO");
	else if (token->type == TOK_AND)
		ret = ft_printf("TOK_AND");
	else if (token->type == TOK_RINOUT0)
		ret = ft_printf("TOK_RINOUT");
	else if (token->type == TOK_RIN1)
		ret = ft_printf("TOK_RIN1");
	else if (token->type == TOK_IFOF)
		ret = ft_printf("TOK_IFOF");
	return (ret);
}

static int	print_tokentype4(t_token *token)
{
	int	ret;

	ret = 0;
	if (token->type == TOK_RIN_FDTO)
		ret = ft_printf("TOK_RIN_FDTO");
	else if (token->type == TOK_IFOF_FD)
		ret = ft_printf("TOK_IFOF_FD");
	else if (token->type == TOK_WHITE)
		ret = ft_printf("TOK_WHITE");
	else if (token->type == TOK_NULL)
		ret = ft_printf("TOK_NULL");
	else if (token->type == TOK_QWORD)
		ret = ft_printf("TOK_QWORD");
	return (ret);
}

void	print_toklst(t_toklst *tlst)
{
	if (!tlst)
	{
		ft_printf("toklst = NULL!\n");
		return ;
	}
	while (tlst)
	{
		ft_printf("token: ");
		if (print_tokentype1(tlst->token))
			;
		else if (print_tokentype2(tlst->token))
			;
		else if (print_tokentype3(tlst->token))
			;
		else
			print_tokentype4(tlst->token);
		ft_printf(", value: `%s`\n", tlst->token->value);
		tlst = tlst->next;
	}
}

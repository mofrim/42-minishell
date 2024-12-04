/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/29 16:42:27 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokenlist	*tokenize(char *input)
{
	t_tokenlist	*toklst;

	toklst = tokenize_lvl1(input);
	if (!toklst)
		return (NULL);
	if (!tokenize_lvl2(toklst))
		return (NULL);
#ifdef DEBUG
	ft_printf("<< DEBUG >> toklist after lvl2:\n");
	print_toklst(toklst);
#endif
	return (toklst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/18 11:03:27 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* If there is an error in lvl2 this means there was a syntax error so, in this
 * case $? has to be set to "2" like in bash. */
t_toklst	*tokenize(char *input, t_envlst **el)
{
	t_toklst	*toklst;

	toklst = tokenize_lvl1(input, *el);
	if (!toklst)
		return (NULL);

#ifdef DEBUG
	ft_printf(RED "<< DEBUG >> toklst after lvl1:\n" RST);
	print_toklst(toklst);
#endif

	if (tokenize_lvl2(&toklst))
	{
		set_env_entry("?", "2", el);
		return (NULL);
	}

#ifdef DEBUG
	ft_printf(RED "<< DEBUG >> toklst after lvl2:\n" RST);
	print_toklst(toklst);
#endif

	if (!tokenize_lvl3(&toklst))
		return (NULL);

#ifdef DEBUG
	ft_printf(RED "<< DEBUG >> toklst after lvl3:\n" RST);
	print_toklst(toklst);
#endif

	return (toklst);
}

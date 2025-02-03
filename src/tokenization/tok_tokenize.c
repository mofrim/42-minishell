/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 11:23:09 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* If there is an error in lvl2 this means there was a syntax error so, in this
 * case $? has to be set to "2" like in bash. */
t_toklst	*tokenize(char *input, t_envlst **el)
{
	t_toklst	*toklst;
	int			retval;

	toklst = tokenize_lvl1(input, *el);
	if (!toklst)
		return (NULL);
	retval = tokenize_lvl2(&toklst);
	if (retval)
	{
		toklst_clear(&toklst);
		if (retval == 2)
			set_env_entry("?", "2", el);
		else
			set_env_entry("?", "0", el);
		return (NULL);
	}
	if (tokenize_lvl3(&toklst))
		return (NULL);
	return (toklst);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:09:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/05 17:21:05 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_toklst	*tokenize(char *input, t_envlst *env)
{
	t_toklst	*toklst;

	toklst = tokenize_lvl1(input, env);
	if (!toklst)
		return (NULL);
	if (!tokenize_lvl2(toklst))
		return (NULL);
	return (toklst);
}

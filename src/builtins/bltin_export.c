/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:50:30 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/03 09:58:14 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Export env-var with name and value to global envlst. Return 0 if everything
 * went fine. Return -1 if f.ex. name was not correctly formatted. */
// TODO implement error checking
int	export(t_envlst **env, char *name, char *val)
{
	t_envlst	*newvar;

	if (!name)
		return (-1);
	newvar = envlst_new(name, val);
	envlst_add_back(env, newvar);
	return (0);
}

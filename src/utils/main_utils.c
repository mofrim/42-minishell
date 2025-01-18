/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 23:45:32 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/19 00:07:10 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Set the ? envvar.
 *
 * Only set it if we are in normal interactive mode or, when minishell gets
 * prompt via pipe, the old value of $? is not 2 indicating a syntax error from
 * before.
 */
void	set_exit_status_envvar(char *status_str, t_envlst **el)
{
	if (isatty(STDIN_FILENO))
		set_env_entry("?", status_str, el);
	else if (ft_atoi(get_env_value("?", *el)) != 2)
			set_env_entry("?", status_str, el);
	free(status_str);
}

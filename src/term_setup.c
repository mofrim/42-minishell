/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:16:59 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/29 10:43:31 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Setup terminal so that Ctrl-\ is completely ignored. Old Terminal settings
 * need to be restored later by a call to tcsetattr(STDIN_FILENO, TCSANOW,
 * old_settings)! */
void	term_setup(t_termios *old_settings)
{
	t_termios	new_settings;

	tcgetattr(STDIN_FILENO, old_settings);
	new_settings = *old_settings;
	new_settings.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}
//// keep for later use maybe... `man tcsetattr`
//
// new_settings.c_lflag |= ISIG;
// new_settings.c_lflag &= ~ICANON;
// new_settings.c_lflag |= ECHO;
// new_settings.c_lflag &= ~ECHOCTL;
// new_settings.c_lflag &= ~NOFLSH;

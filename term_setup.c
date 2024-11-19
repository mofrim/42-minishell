/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:16:59 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/19 10:24:49 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* For tcsetattr, tcgetattr, ISIG, VQUIT, ... */
#include <termios.h>

/* For STDIN_FILENO, _POSIX_VDISABLE */
#include <unistd.h>

/* Setup terminal so that Ctrl-\ is completely ignored. Old Terminal settings
 * need to be restored later by a call to tcsetattr(STDIN_FILENO, TCSANOW,
 * old_settings)! */
void	term_setup(t_termios *old_settings)
{
	t_termios new_settings;

	tcgetattr(STDIN_FILENO, old_settings);
	new_settings = *old_settings;
	// new_settings.c_lflag |= ISIG;
	// new_settings.c_lflag &= ~ICANON;
	// new_settings.c_lflag |= ECHO;
	// new_settings.c_lflag &= ~ECHOCTL;
	// new_settings.c_lflag &= ~NOFLSH;
    new_settings.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}

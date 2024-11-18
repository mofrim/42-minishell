/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 20:44:43 by fmaurer           #+#    #+#             */
/*   Updated: 2024/11/18 18:29:52 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include <signal.h>

# define PROMPT "$"

typedef struct termios t_termios;

void	signal_handler(int signum);
void	signal_setup(void (*sig_handler)(int));
void	term_setup(struct termios *old_settings);

void	nullcheck(void *p, char *msg);
void	error_exit(char *msg);

#endif

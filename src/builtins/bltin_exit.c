/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:13:58 by fmaurer           #+#    #+#             */
/*   Updated: 2025/02/03 10:11:19 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long	get_long_numstr(char *s);
static int	check_numstr_long(char *s);

/* Rules for exit:
 *
 * 1) if there is no arg -> exit and return 0
 * 2) if there is one arg and it is a valid long number -> exit and return that
 * number (maybe cast to unsigned char?!?!)
 * 3) if there are 2 args and the first one is a valid long number: do nothing,
 * print error msg "too many args" and return 1
 * 4) if there are 2 args (or more) and the first one is already not valid long
 * -> do the exit but print error msg "numeric arg required" and return 2
 * 5) the same holds for only one numerical arg which is to big or small
 */

/* Do the actual exit_flag setting which needs to be available to parent
 * process of course. Therefore needed to make the cmdlst bidirectional in order
 * to find the head of the list to set the flag there. */
int	bltin_exit_preout(t_cmdlst *cl, t_envlst **el)
{
	(void)el;
	if (cl->arg_count == 1)
		cl->exit_flag = 1;
	else if (cl->arg_count == 2 && check_numstr_long(cl->args[1]) >= 0 && \
			!cmdlst_head(cl)->next)
		cmdlst_head(cl)->exit_flag = 1;
	return (0);
}

int	bltin_exit_out(t_cmdlst *cl, t_envlst **el)
{
	(void)el;
	(void)cl;
	if (cl->arg_count == 2 && check_numstr_long(cl->args[1]) >= 0)
		return (get_long_numstr(cl->args[1]));
	if (cl->arg_count >= 2 && check_numstr_long(cl->args[1]) == -1)
		return (minish_errormsg2("exit", cl->args[1],
				"numeric argument required", 2));
	if (cl->arg_count > 2)
		return (minish_errormsg("exit", "too many arguments", 1));
	return (0);
}

/**
 * Check the numstr s before conversion to long value.
 * Return values:
 * -1: not a valid long numstr
 *  0: positive long numstr
 *  1: negative long numstr 
 */
static int	check_numstr_long(char *s)
{
	long	num;
	long	step;
	long	neg;

	neg = 0;
	num = 0;
	if (!s || !*s)
		return (-1);
	if (*s == '-' || *s == '+')
	{
		neg = (*s == '-');
		s++;
	}
	while (ft_isdigit(*s))
	{
		step = num * 9 + *s++ - '0';
		if (num && step > !neg * (LONG_MAX - num) + neg * (LONG_MAX - num + 1))
			return (-1);
		num += step;
	}
	if (!ft_isdigit(*s) && *s)
		return (-1);
	return (neg);
}

/**
 * Do the conversion from numstring to long value.
 *
 * Always call check_numstr_long() before this, because there is no error
 * checking inside this function anymore!
 */
static long	get_long_numstr(char *s)
{
	long	num;
	int		neg;

	neg = 0;
	num = 0;
	if (*s == '-' || *s == '+')
	{
		neg = (*s == '-');
		s++;
	}
	while (ft_isdigit(*s))
		num = (num * 10) + (*s++ - '0');
	if (neg)
		return (-num);
	return (num);
}

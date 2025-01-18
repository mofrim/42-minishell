/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:10:05 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/06 17:26:51 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

/* Free a NULL terminated (char **) array. */
void	free_ptr2ptr(char ***ptr)
{
	char	**tmp;

	tmp = *ptr;
	while (**ptr)
	{
		free(**ptr);
		(*ptr)++;
	}
	free(tmp);
	tmp = NULL;
}

/* Check if char argument is a number in [0,9] or not. Return 1 if true, 0
 * else.*/
int	ft_isnum(char c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

/* Determine wether a string represents a positive int number. Return its value
 * if true, -1 otherwise. */
int	get_posint_numstr(char *s)
{
	long	num;

	num = 0;
	while (ft_isdigit(*s))
	{
		num = num * 10 + *s++ - '0';
		if (num > INT_MAX)
			return (-1);
	}
	if (!ft_isdigit(*s) && *s)
		return (-1);
	return (num);
}

/* Return the size of a char** array returned from a ft_split call. */
int	splitsize(char **s)
{
	int	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

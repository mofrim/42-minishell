/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:14:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/22 22:41:04 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bltin_pwd(t_cmdlst *cl, t_envlst **el)
{
	char	*str;

	(void)cl;
	(void)el;
	str = ft_calloc(1024, sizeof(char));
	ft_printf("%s\n", getcwd(str, 500));
	free(str);
	return (0);
}

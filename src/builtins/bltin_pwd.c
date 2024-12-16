/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bltin_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpah <elpah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:14:10 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 10:33:45 by elpah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bltin_pwd(void)
{
	char	*str;

	str = ft_calloc(1024, sizeof(char));
	ft_printf("%s\n", getcwd(str, 500));
	free(str);
	return (0);
}

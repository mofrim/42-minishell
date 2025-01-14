/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 20:54:46 by fmaurer           #+#    #+#             */
/*   Updated: 2025/01/14 11:24:08 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME maybe do a little error checking
void	heredoc_cleanup(t_cmdlst *cl)
{
	t_redirlst	*rl;

	while (cl)
	{
		if (cl->heredocs)
		{
			rl = cl->redirs;
			while (rl)
			{
				if (rl->redtype == RE_HERE)
					unlink(rl->infile);
				rl = rl->next;
			}
		}
		cl = cl->next;
	}
}

static void	replace_var(int offset, int i, char **input, t_envlst *el)
{
	char	*before_var;
	char	*after_var;
	char	*var_name;
	char	*tmp;

	before_var = ft_substr(*input, 0, offset);
	after_var = ft_substr(*input, offset + i, ft_strlen(*input) - offset - i);
	var_name = ft_substr(*input, offset + 1, i - 1);
	tmp = before_var;
	before_var = ft_strjoin(before_var, get_env_value(var_name, el));
	free(tmp);
	tmp = before_var;
	before_var = ft_strjoin(before_var, after_var);
	free(tmp);
	tmp = *input;
	*input = before_var;
	free(tmp);
	free(after_var);
	free(var_name);
}

static int	is_special_varchar(char c)
{
	if (c == '?')
		return (1);
	return (0);
}

void	substitute_envvars(char **prompt_input, t_envlst *el)
{
	char	*input;
	char	*varsym;
	int		i;

	input = *prompt_input;
	varsym = ft_strchr(input, '$');
	while (varsym && varsym[1])
	{
		i = 1;
		while (is_valid_var_name(varsym[i]) && !is_special_varchar(varsym[i]))
			i++;
		if (is_special_varchar(varsym[1]))
			i = 2;
		if (i != 1)
		{
			replace_var(varsym - input, i, prompt_input, el);
			input = *prompt_input;
		}
		else
			input++;
		varsym = ft_strchr(input, '$');
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   par_redirlst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 19:26:24 by fmaurer           #+#    #+#             */
/*   Updated: 2024/12/16 21:54:34 by fmaurer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Create new redirlst entry function. Too much stuff to set. Will have to do
 * this somehow externally. Using a t_redirlst for that! */
t_redirlst	*redirlst_new(t_redirlst new)
{
	t_redirlst	*nn;

	nn = (t_redirlst *) malloc(sizeof(t_redirlst));
	if (!nn)
		return (NULL);
	nn->redtype = new.redtype;
	nn->fd_in = new.fd_in;
	nn->fd_out = new.fd_out;
	nn->outfile = ft_strdup(new.outfile);
	nn->infile = ft_strdup(new.infile);
	nn->next = NULL;
	return (nn);
}

t_redirlst	*redirlst_last(t_redirlst *head)
{
	t_redirlst	*cur;

	if (!head)
		return (NULL);
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void	redirlst_add_back(t_redirlst **head, t_redirlst *newend)
{
	t_redirlst	*oldend;

	if (!newend)
		return ;
	if (!*head)
	{
		*head = newend;
		return ;
	}
	oldend = redirlst_last(*head);
	oldend->next = newend;
}

void	redirlst_clear(t_redirlst **lst)
{
	t_redirlst	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->infile)
			free((*lst)->infile);
		if ((*lst)->outfile)
			free((*lst)->outfile);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	redirlst_print(t_redirlst *lst)
{
	while (lst)
	{
		ft_printf("{type: %d, ", lst->redtype);
		ft_printf("fd_in: %d, ", lst->fd_in);
		ft_printf("fd_out: %d, ", lst->fd_out);
		ft_printf("infile: %s, ", lst->infile);
		ft_printf("outfile: %s}\n", lst->outfile);
		lst = lst->next;
	}
}

/* Initialize a variable of type t_redirlst to some good old default values. */
void	init_redirlst_var(t_redirlst *var)
{
	var->redtype = 0;
	var->fd_in = 0;
	var->fd_out = 0;
	var->outfile = NULL;
	var->infile = NULL;
	var->next = NULL;
}

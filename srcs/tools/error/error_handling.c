/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:42:41 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/13 11:42:44 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_empty(char *ft)
{
	ft_putstr(RED);
	ft_putstr(ft);
	ft_putstr(BLANK);
	ft_putstr(": ");
	ft_putendl("no environment variables");
}

void	print_quotes(char *arg)
{
	ft_putstr(RED);
	ft_putstr(arg);
	ft_putstr(BLANK);
	ft_putstr(": ");
	ft_putendl("quote error");
}

void	print_ft_found(char *ft)
{
	ft_putstr(RED);
	ft_putstr(ft);
	ft_putstr(BLANK);
	ft_putstr(": ");
	ft_putendl("command not found");
}

void	print_args(char *ft)
{
	ft_putstr(RED);
	ft_putstr(ft);
	ft_putstr(BLANK);
	ft_putstr(": ");
	ft_putendl("arguments problem");
}

void	print_var_found(char *ft, char *arg)
{
	ft_putstr(RED);
	ft_putstr(ft);
	ft_putstr(BLANK);
	ft_putstr(": ");
	ft_putstr(arg);
	ft_putendl(" no such a variable");
}

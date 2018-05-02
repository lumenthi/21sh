/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:42:41 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/02 13:57:05 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_empty(char *ft)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(ft, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("no environment variables", 2);
}

void	print_quotes(char *arg)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("quote error", 2);
}

void	print_ft_found(char *ft)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(ft, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("command not found", 2);
}

void	print_args(char *ft)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(ft, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("arguments problem", 2);
}

void	print_var_found(char *ft, char *arg)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(ft, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(" no such a variable", 2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:40:30 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 12:39:03 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	ft_print_error(char *ft, int error, char *arg)
{
	if (error == EMPTY)
		print_empty(ft);
	else if (error == QUOTES)
		print_quotes(arg);
	else if (error == FT_FOUND)
		print_ft_found(ft);
	else if (error == ARGS)
		print_args(ft);
	else if (error == VAR_FOUND)
		print_var_found(ft, arg);
}

void	parse_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": parse error\n", 2);
}

void	fd_error(char *str)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": not a valid fd\n", 2);
}

void	input_error(char *line)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": no such file or directory\n", 2);
}

void	history_error(void)
{
	g_history->error = 1;
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": cant initialize history\n", 2);
}

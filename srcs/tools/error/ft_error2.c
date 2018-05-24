/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 16:02:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 16:16:22 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	pipe_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": fatal pipe error, exiting\n", 2);
	exit(-1);
}

void	malloc_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": fatal malloc error, exiting\n", 2);
	exit(-1);
}

void	dup_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": fatal dup error, exiting\n", 2);
	exit(-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 19:00:43 by lumenthi          #+#    #+#             */
/*   Updated: 2018/04/05 10:52:50 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_echo(char **args)
{
	int		i;
	int		f;

	i = 1;
	f = 0;
	while (args[i])
	{
		ft_putstr(args[i]);
		if (args[i + 1] && f == 0)
			ft_putchar(' ');
		f = 0;
		i++;
	}
	ft_putchar('\n');
}

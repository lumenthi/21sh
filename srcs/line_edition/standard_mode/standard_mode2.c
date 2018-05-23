/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standard_mode2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:13:46 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:17:36 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

int		standard_enter(int i)
{
	ft_end(i);
	ft_putchar('\n');
	return (1);
}

int		standard_sigint(int i)
{
	ft_end(i);
	if (g_data->line)
		free(g_data->line);
	ft_putchar('\n');
	return (1);
}

int		standard_ctrld(int *i)
{
	if (ft_strlen(g_data->line) == 0)
	{
		g_history->special = 1;
		ft_putstr("exit");
		return (1);
	}
	if (g_data->pos < *i)
		edit_line(i);
	return (0);
}

int		shortcut_clear(void)
{
	if (g_data->line)
		free(g_data->line);
	g_history->special = 1;
	return (1);
}

int		shortcut_echap(int i)
{
	ft_end(i);
	if (g_data->line)
		free(g_data->line);
	g_history->special = 1;
	ft_putstr("exit\n");
	return (1);
}

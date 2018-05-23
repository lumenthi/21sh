/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_mode2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:49:24 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:50:29 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

int		quote_leave(int *i, char mode)
{
	ft_end(*i);
	inser_char(mode, i);
	ft_putchar('\n');
	return (1);
}

int		quote_ctrld(int *i)
{
	if (ft_strlen(g_data->line) == 0)
	{
		ft_end(*i);
		free(g_data->line);
		g_data->line = NULL;
		ft_putchar('\n');
		return (1);
	}
	if (g_data->pos < *i)
		edit_line(i);
	return (0);
}

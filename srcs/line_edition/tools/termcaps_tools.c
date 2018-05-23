/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 11:24:18 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:01:14 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

int		my_outc(int c)
{
	write(0, &c, 1);
	return (1);
}

void	ft_put(char *str)
{
	tputs(tgetstr(str, NULL), 0, my_outc);
}

void	get_winsize(void)
{
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	g_data->w_row = w.ws_row;
	g_data->w_col = w.ws_col;
}

void	move_cursor(int x, int y)
{
	tputs(tgoto(tgetstr("cm", NULL), x, y), 0, my_outc);
	g_data->cursor->x = x;
	g_data->cursor->y = y;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:12:55 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:12:29 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

static int	ft_left(int x)
{
	if (x == 0)
	{
		ft_put("up");
		tputs(tgoto(tgetstr("ch", NULL), 0, g_data->w_col - 1), 0, my_outc);
		g_data->pos--;
		g_data->cursor->y--;
		if (!g_data->cursor->y)
			g_data->cursor->x = g_data->w_col - g_data->cursor->start;
		else
			g_data->cursor->x = g_data->w_col - 1;
		return (1);
	}
	else if (g_data->pos > 0)
	{
		ft_put("le");
		g_data->pos--;
		g_data->cursor->x--;
		return (1);
	}
	return (0);
}

static int	ft_right(int x, int i)
{
	if (x == g_data->w_col - 1)
	{
		ft_put("cr");
		ft_put("do");
		g_data->cursor->x = 0;
		g_data->pos++;
		g_data->cursor->y++;
		return (1);
	}
	else if (g_data->pos < i)
	{
		ft_put("nd");
		g_data->pos++;
		g_data->cursor->x++;
		return (1);
	}
	return (0);
}

int			ft_move(char dir, int i)
{
	int	start;
	int	x;

	start = g_data->cursor->y ? 0 : (g_data->cursor->start - 1);
	x = g_data->cursor->x + start;
	if (dir == 'l')
		return (ft_left(x));
	else if (dir == 'r')
		return (ft_right(x, i));
	return (0);
}

void		ft_print(int len)
{
	int i;
	int pos;

	i = g_data->pos;
	pos = i;
	while (g_data->line[i])
	{
		ft_putchar(g_data->line[i]);
		ft_put("le");
		ft_move('r', len);
		i++;
	}
	i ? --i : i;
	while (i >= pos)
	{
		ft_move('l', len);
		i--;
	}
}

void		edit_line(int *i)
{
	ft_put("cd");
	g_data->line = ft_delete(g_data->line, g_data->pos, *i);
	ft_print(*i);
	(*i) == 0 ? *i : (*i)--;
}

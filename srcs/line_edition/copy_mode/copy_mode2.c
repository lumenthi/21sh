/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_mode2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:24:50 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:30:25 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	copy_sub(int len, int *i)
{
	while (len)
	{
		if (ft_move('l', *i))
			edit_line(i);
		len--;
	}
}

void	copy_write(int pos, int *i)
{
	ft_clear(*i);
	ft_rewrite(i);
	while (g_data->pos > pos)
		ft_move('l', *i);
}

void	mode_icon(char icon, int i)
{
	int		x;
	int		y;
	int		pos;

	x = g_data->cursor->x;
	y = g_data->cursor->y;
	pos = g_data->pos;
	ft_put("sc");
	while (ft_move('l', i))
		;
	g_data->cursor->x = g_data->cursor->x + g_data->cursor->start;
	tputs(tgoto(tgetstr("ch", NULL), 0, g_data->cursor->x - 2), 0, my_outc);
	ft_putchar(icon);
	ft_put("rc");
	g_data->pos = pos;
	g_data->cursor->x = x;
	g_data->cursor->y = y;
}

int		copy_sigint(int *i)
{
	ft_end(*i);
	if (g_data->line)
		free(g_data->line);
	g_data->line = NULL;
	ft_putchar('\n');
	print_prompt(g_data->cpy);
	*i = 0;
	return (1);
}

void	copy_cpy(int *i, int *select)
{
	mode_icon('P', *i);
	copy_write(g_data->pos, i);
	*select = 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_tools3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:06:30 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:09:16 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

char	*insert_str(char *line, char *ins, int pos, int i)
{
	int		j;

	j = 0;
	while (ins[j])
	{
		line = ft_insert(line, ins[j], pos, i);
		pos++;
		j++;
		i++;
	}
	return (line);
}

void	ft_clear(int i)
{
	while (ft_move('l', i))
		;
	ft_put("cd");
}

void	inser_char(char buf, int *i)
{
	ft_put("cd");
	g_data->line = ft_insert(g_data->line, buf, g_data->pos, *i);
	(*i)++;
	ft_print(*i);
	ft_move('r', *i);
}

void	ft_rewrite(int *i)
{
	ft_clear(*i);
	g_data->line ? ft_putstr(g_data->line) : 0;
	*i = ft_strlen(g_data->line);
	g_data->pos = *i;
	g_data->cursor->y = (g_data->pos + g_data->cursor->start) / g_data->w_col;
	if (!g_data->cursor->y)
		g_data->cursor->x = g_data->pos;
	else
	{
		g_data->cursor->x = (g_data->pos + g_data->cursor->start - 1)
		- (g_data->cursor->y * g_data->w_col);
	}
}

void	ft_home(int i)
{
	while (ft_move('l', i))
		;
}

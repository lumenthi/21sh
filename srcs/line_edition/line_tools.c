/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:12:55 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 14:37:01 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/21sh.h"

int		ft_move(char dir, int i)
{
	int	start;
	int	x;

	start = g_data->cursor->y ? 0 : (g_data->cursor->start - 1);
	x = g_data->cursor->x + start;
	if (dir == 'l')
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
	else if (dir == 'r')
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
	return (0);
}

static void	ft_print(int len)
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

void	edit_line(int *i)
{
	ft_put("cd");
	g_data->line = ft_delete(g_data->line, g_data->pos, *i);
	ft_print(*i);
	(*i) == 0 ? *i : (*i)--;
}

static char	*ft_insert(char *line, char buf, int pos, int i)
{
	char	*after;

	if (i == 0)
	{
		free(line);
		line = ft_strdup("");
	}
	else
		line[i] = '\0';
	if (!(after = malloc(i + 2)))
		exit(-1);
	ft_strncpy(after, line, pos);
	after[pos] = buf;
	after[pos + 1] = '\0';
	ft_strcat(after, line + pos);
	after[i + 1] = '\0';
	if (line)
		free(line);
	return (after);
}

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
		g_data->cursor->x = (g_data->pos + g_data->cursor->start - 1) - (g_data->cursor->y * g_data->w_col);
}

void	ft_home(int i)
{
	while (ft_move('l', i))
		;
}

void	ft_end(int i)
{
	while (ft_move('r', i))
		;
}

void	line_up(int i)
{
	int j;

	j = 0;
	while (j != g_data->w_col)
	{
		if (!ft_move('l', i))
			break ;
		j++;
	}
}

void	line_down(int i)
{
	int j;

	j = 0;
	while (j != g_data->w_col)
	{
		if (!ft_move('r', i))
			break ;
		j++;
	}
}

void	word_left(int i)
{
	while (ft_move('l', i))
	{
		if (ft_isalnum(g_data->line[g_data->pos]) &&
			ft_isspace(g_data->line[g_data->pos + 1]))
			break ;
	}
}

void	word_right(int i)
{
	while (ft_move('r', i))
	{
		if (ft_isalnum(g_data->line[g_data->pos]) &&
			ft_isspace(g_data->line[g_data->pos - 1]))
			break ;
	}
}

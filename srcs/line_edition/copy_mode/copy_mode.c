/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:18:58 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 14:36:53 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static void	ft_sub(int len, int *i)
{
	while (len)
	{
		if (ft_move('l', *i))
			edit_line(i);
		len--;
	}
}

static void	ft_write(int pos, int *i)
{
	ft_clear(*i);
	ft_rewrite(i);
	while (g_data->pos > pos)
		ft_move('l', *i);
}

static void	mode_icon(char icon, int i)
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

void	copy_mode(int *i)
{
	char	buf[20];
	int		select = 0;
	char	*cpy = ft_strdup("");
	char	*tmp;

	mode_icon('C', *i);
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if (CTRL_C)
		{
			ft_end(*i);
			if (g_data->line)
				free(g_data->line);
			g_data->line = NULL;
			ft_putchar('\n');
			print_prompt(g_data->cpy);
			*i = 0;
			break ;
		}
		else if ((RIGHT||SPACE) && g_data->line)
		{
			if (g_data->line[g_data->pos])
			{
				if (!select)
				{
					ft_put("us");
					tmp = ft_charjoin(cpy, g_data->line[g_data->pos]);
//					ft_putstr_fd("free1\n", 2);
					free(cpy);
					cpy = ft_strdup(tmp);
					free(tmp);
				}
				if (ft_move('r', *i))
				{
					ft_move('l', *i);
					ft_put("sc");
					ft_putchar(g_data->line[g_data->pos]);
					ft_put("rc");
					ft_move('r', *i);
				}
				ft_put("ue");
			}
		}
		else if (HOME && select)
			ft_home(*i);
		else if (END && select)
			ft_end(*i);
		else if (A_RIGHT && select)
			word_right(*i);
		else if (A_LEFT && select)
			word_left(*i);
		else if (A_DOWN && select)
			line_down(*i);
		else if (A_UP && select)
			line_up(*i);
		else if (buf[0] == 'd' && !select)
		{
			mode_icon('P', *i);
			ft_sub(ft_strlen(cpy), i);
			ft_write(g_data->pos, i);
			select = 1;
		}
		else if (buf[0] == 'y' && !select)
		{
			mode_icon('P', *i);
			ft_write(g_data->pos, i);
			select = 1;
		}
		else if (buf[0] == 'p' && select)
		{
			g_data->line = insert_str(g_data->line, cpy, g_data->pos, *i);
			select = 0;
			ft_clear(*i);
			*i = (int)ft_strlen(g_data->line);
			ft_rewrite(i);
			break ;
		}
		else if (LEFT || BACKSPACE)
		{
			if (cpy && !select)
				cpy = ft_delete(cpy, ft_strlen(cpy) - 1, ft_strlen(cpy));
			if (ft_move('l', *i))
			{
				ft_put("sc");
				ft_put("ue");
				ft_putchar(g_data->line[g_data->pos]);
				ft_put("rc");
			}
		}
		else if (A_C || ECHAP)
		{
			if (select)
				ft_put("ue");
			ft_write(g_data->pos, i);
			break ;
		}
	}
	free(cpy);
	mode_icon(' ', *i);
}

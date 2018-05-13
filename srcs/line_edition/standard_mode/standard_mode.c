/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standard_mode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:22:05 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 14:23:04 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

char	*gnl(void)
{
	char	buf[20];
	int		i;

	g_data->line = NULL;
	ft_put("ks");
	i = 0;
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
//		print_key(buf);
		if (ENTER)
		{
			ft_end(i);
			ft_putchar('\n');
			break ;
		}
		else if (CTRL_C)
		{
			ft_end(i);
			if (g_data->line)
				free(g_data->line);
			ft_putchar('\n');
			return (ft_strdup(""));
		}
		else if (CTRL_D)
		{
		//	printf("g_data->pos: %d, i: %d\n", g_data->pos, i);
			if (ft_strlen(g_data->line) == 0)
			{
				g_history->special = 1;
				ft_putstr("exit");
				return (ft_strdup("exit"));
			}
			if (g_data->pos < i)
				edit_line(&i);
		}
		else if (BACKSPACE)
		{
			if (ft_move('l', i))
				edit_line(&i);
		}
		else if (CTRL_L)
		{
			if (g_data->line)
				free(g_data->line);
			g_history->special = 1;
			return (ft_strdup("clear"));
		}
		else if (HOME)
			ft_home(i);
		else if (A_C)
			copy_mode(&i);
		else if (END)
			ft_end(i);
		else if (LEFT)
			ft_move('l', i);
		else if (RIGHT)
			ft_move('r', i);
		else if (UP)
			g_history->nb_lines ? history_search(&i, 'u') : 0;
		else if (A_RIGHT)
			word_right(i);
		else if (A_LEFT)
			word_left(i);
		else if (A_DOWN)
			line_down(i);
		else if (A_UP)
			line_up(i);
		else if (DOWN)
			g_history->nb_lines ? history_search(&i, 'd') : 0;
		else if (ECHAP)
		{
			if (g_data->line)
				free(g_data->line);
			g_history->special = 1;
			ft_putstr("exit\n");
			return (ft_strdup("exit"));
		}
		else if (ft_isprintable(buf[0]))
			inser_char(buf[0], &i);
	}
	ft_put("ke");
	return (g_data->line);
}

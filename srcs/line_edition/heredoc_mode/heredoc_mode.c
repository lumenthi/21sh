/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:25:42 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 14:26:30 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

void		write_mode(int fd, char *lim)
{
	char	buf[20];
	int		i;

	ft_put("ks");
	free(g_data->line);
	g_data->line = NULL;
	i = 0;
	g_data->pos = 0;
	g_data->cursor->y = 0;
	g_data->cursor->x = 0;
	ft_putstr("heredoc> ");
	g_data->cursor->start = 10;
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if (CTRL_C)
		{
			ft_end(i);
			free(g_data->line);
			g_data->line = NULL;
			g_data->error = 1;
			ft_putchar('\n');
			break ;
		}
		else if (BACKSPACE)
		{
			if (ft_move('l', i))
				edit_line(&i);
		}
		else if (CTRL_D)
		{
		//	printf("g_data->pos: %d, i: %d\n", g_data->pos, i);
			if (ft_strlen(g_data->line) == 0 || g_data->line == NULL)
			{
				inser_char('\n', &i);
				ft_put("le");
				break ;
			}
			else if (g_data->pos < i)
				edit_line(&i);
		}
		else if (ENTER)
		{
			if (!g_data->line)
				g_data->line = ft_strdup("");
			ft_end(i);
			if (ft_strcmp(g_data->line, lim) != 0)
			{
				inser_char('\n', &i);
				ft_putstr_fd(g_data->line, fd);
				ft_put("le");
				write_mode(fd, lim);
			}
			else
			{
				inser_char('\n', &i);
				ft_put("le");
			}
			break ;
		}
		else if (LEFT)
			ft_move('l', i);
		else if (RIGHT)
			ft_move('r', i);
		else if (ft_isprintable(buf[0]))
			inser_char(buf[0], &i);
	}
	ft_put("ke");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:25:42 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/13 22:32:40 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static void	heredoc_init(int *i)
{
	ft_put("ks");
	free(g_data->line);
	g_data->line = NULL;
	*i = 0;
	g_data->pos = 0;
	g_data->cursor->y = 0;
	g_data->cursor->x = 0;
	ft_putstr("heredoc> ");
	g_data->cursor->start = 10;
}

static int	sigint_handler(int i)
{
	ft_end(i);
	free(g_data->line);
	g_data->line = NULL;
	g_data->error = 1;
	ft_putchar('\n');
	return (1);
}

static int	ctrld_handler(int *i)
{
	if (ft_strlen(g_data->line) == 0 || g_data->line == NULL)
	{
		inser_char('\n', i);
		ft_put("le");
		return (1);
	}
	else if (g_data->pos < *i)
		edit_line(i);
	return (0);
}

static int	ft_enter(char *lim, int *i, int fd)
{
	if (!g_data->line)
		g_data->line = ft_strdup("");
	ft_end(*i);
	if (ft_strcmp(g_data->line, lim) != 0)
	{
		inser_char('\n', i);
		ft_putstr_fd(g_data->line, fd);
		ft_put("le");
		write_mode(fd, lim);
	}
	else
	{
		inser_char('\n', i);
		ft_put("le");
	}
	return (1);
}

void		write_mode(int fd, char *lim)
{
	char	buf[20];
	int		i;

	heredoc_init(&i);
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if ((CTRL_C && sigint_handler(i)) || (CTRL_D && ctrld_handler(&i)))
			break ;
		else if (BACKSPACE && ft_move('l', i))
			edit_line(&i);
		else if (ENTER && ft_enter(lim, &i, fd))
			break ;
		else if (LEFT)
			ft_move('l', i);
		else if (RIGHT)
			ft_move('r', i);
		else if (ft_isprintable(buf[0]))
			inser_char(buf[0], &i);
	}
	ft_put("ke");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_mode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:18:58 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:39:41 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

static void	ft_right(int *i, char **cpy, int select)
{
	char	*tmp;

	if (g_data->line[g_data->pos])
	{
		if (!select)
		{
			ft_put("us");
			tmp = ft_charjoin(*cpy, g_data->line[g_data->pos]);
			free(*cpy);
			*cpy = ft_strdup(tmp);
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

static void	ft_left(int *i, char **cpy, int select)
{
	if (cpy && !select)
		*cpy = ft_delete(*cpy, ft_strlen(*cpy) - 1, ft_strlen(*cpy));
	if (ft_move('l', *i))
	{
		ft_put("sc");
		ft_put("ue");
		ft_putchar(g_data->line[g_data->pos]);
		ft_put("rc");
	}
}

static int	copy_leave(int *i, int select, char *cpy, char *buf)
{
	if (CTRL_C && copy_sigint(i))
		return (1);
	else if ((buf[0] == 'p' && select) && copy_paste(i, &cpy, &select))
		return (1);
	else if ((A_C || ECHAP) && copy_quit(i, select))
		return (1);
	return (0);
}

static void	copy_operations(int *i, char **cpy, int *select, char *buf)
{
	if ((RIGHT || SPACE) && g_data->line)
		ft_right(i, cpy, *select);
	else if (HOME && *select)
		ft_home(*i);
	else if (END && *select)
		ft_end(*i);
	else if (A_RIGHT && *select)
		word_right(*i);
	else if (A_LEFT && *select)
		word_left(*i);
	else if (A_DOWN && *select)
		line_down(*i);
	else if (A_UP && *select)
		line_up(*i);
	else if (buf[0] == 'd' && !*select)
		copy_cut(i, cpy, select);
	else if (buf[0] == 'y' && !*select)
		copy_cpy(i, select);
	else if (LEFT || BACKSPACE)
		ft_left(i, cpy, *select);
}

void		copy_mode(int *i)
{
	char	buf[20];
	int		select;
	char	*cpy;

	select = 0;
	cpy = ft_strdup("");
	mode_icon('C', *i);
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if (copy_leave(i, select, cpy, buf))
			break ;
		else
			copy_operations(i, &cpy, &select, buf);
	}
	free(cpy);
	mode_icon(' ', *i);
}

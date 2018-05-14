/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_mode.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:23:54 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/14 11:20:10 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static void	quote_init(int *i, char mode)
{
	ft_put("ks");
	free(g_data->line);
	g_data->line = NULL;
	*i = 0;
	g_data->pos = 0;
	g_data->cursor->y = 0;
	g_data->cursor->x = 0;
	if (mode == 34)
	{
		ft_putstr("dquote> ");
		g_data->cursor->start = 9;
	}
	else if (mode == 39)
	{
		ft_putstr("quote> ");
		g_data->cursor->start = 8;
	}
}

static int	sigint_handler(int i)
{
	ft_end(i);
	free(g_data->line);
	g_data->line = NULL;
	ft_putchar('\n');
	return (1);
}

static int	quote_leave(int *i, char mode)
{
	ft_end(*i);
	inser_char(mode, i);
	ft_putchar('\n');
	return (1);
}

static int	ctrld_handler(int *i)
{
	if (ft_strlen(g_data->line) == 0)
	{
		ft_end(*i);
		free(g_data->line);
		g_data->line = NULL;
		ft_putchar('\n');
		return (1);
	}
	if (g_data->pos < *i)
		edit_line(i);
	return (0);
}

static int ft_enter(int *i, char mode)
{
	char	*bu;
	char	*new;

	ft_end(*i);
	inser_char('\n', i);
	ft_put("le");
	bu = ft_strdup(g_data->line);
	new = quote_mode(mode);
	g_data->line = ft_strjoin(bu, new);
	free(bu);
	free(new);
	return (1);
}

static void	quote_actions(int *i, char *buf)
{
	if (HOME)
		ft_home(*i);
	else if (END)
		ft_end(*i);
	else if (LEFT)
		ft_move('l', *i);
	else if (RIGHT)
		ft_move('r', *i);
	else if (A_RIGHT)
		word_right(*i);
	else if (A_LEFT)
		word_left(*i);
	else if (A_DOWN)
		line_down(*i);
	else if (A_UP)
		line_up(*i);
	else if (ft_isprintable(buf[0]))
		inser_char(buf[0], i);
}

char	*quote_mode(char mode)
{
	char	buf[20];
	int		i;

	quote_init(&i, mode);
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if ((CTRL_C && sigint_handler(i)) || (CTRL_D && ctrld_handler(&i)))
			break ;
		else if (buf[0] == mode && quote_leave(&i ,mode))
			break ;
		else if (BACKSPACE && ft_move('l', i))
			edit_line(&i);
		else if (ENTER && ft_enter(&i, mode))
			break ;
		else
			quote_actions(&i, buf);
	}
	ft_put("ke");
	return (g_data->line);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   standard_mode.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:22:05 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:21:57 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

static void	standard_history(int *i, char *buf)
{
	if (UP)
		g_history->nb_lines ? history_search(i, 'u') : 0;
	else if (DOWN)
		g_history->nb_lines ? history_search(i, 'd') : 0;
}

static void	standard_actions(int *i, char *buf)
{
	standard_history(i, buf);
	if (HOME)
		ft_home(*i);
	else if (A_C)
		copy_mode(i);
	else if (END)
		ft_end(*i);
	else if (LEFT)
		ft_move('l', *i);
	else if (RIGHT)
		ft_move('r', *i);
	else if (BACKSPACE && ft_move('l', *i))
		edit_line(i);
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

static void	buf_init(char *ptr1, char *ptr2, char *ptr3)
{
	*ptr1 = 0;
	*ptr2 = 0;
	*ptr3 = 0;
}

static void	gnl_init(int *i)
{
	g_data->line = NULL;
	ft_put("ks");
	*i = 0;
}

char		*gnl(void)
{
	char	buf[20];
	int		i;

	gnl_init(&i);
	while (1)
	{
		buf_init(&buf[0], &buf[1], &buf[2]);
		read(0, buf, 20);
		if (ENTER && standard_enter(i))
			break ;
		else if (CTRL_C && standard_sigint(i))
			return (ft_strdup(""));
		else if (CTRL_D && standard_ctrld(&i))
			return (ft_strdup("exit"));
		else if (CTRL_L && shortcut_clear())
			return (ft_strdup("clear"));
		else if (ECHAP && shortcut_echap(i))
			return (ft_strdup("exit"));
		else
			standard_actions(&i, buf);
	}
	ft_put("ke");
	return (g_data->line);
}

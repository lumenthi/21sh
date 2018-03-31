/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_tools2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:12:55 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/31 16:06:33 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**tab_conv(char **args, char **env)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '$' && var_conv(args[i], env))
		{
			free(args[i]);
			args[i] = ft_strdup(var_conv(args[i], env));
		}
		i++;
	}
	return (args);
}

char	*make_string(char *fullpath)
{
	char *tmp;
	char *str;

	if (!fullpath)
		return (NULL);
	str = ft_strdup(fullpath);
	tmp = ft_strchr(str, ':');
	if (!tmp)
	{
		free(str);
		return (NULL);
	}
	*(tmp) = '\0';
	return (str);
}

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

char	*ft_delete(char *line, int pos, int i)
{
	char	*after;

	after = NULL;
	line[i] = '\0';
	after = malloc(i);
	ft_strncpy(after, line, pos);
	after[pos] = '\0';
	ft_strcat(after, line + (pos + 1));
	after[i] = '\0';
	free(line);
	return (after);
}

void	ft_print(int len)
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
	(*i) == 1 ? *i : (*i)--;
}

char	*ft_insert(char *line, char buf, int pos, int i)
{
	char	*after;

	if (i == 0)
	{
		if (line)
			free(line);
		line = ft_strdup("");
	}
	else
		line[i] = '\0';
	after = malloc(i + 2);
	ft_strncpy(after, line, pos);
	after[pos] = buf;
	after[pos + 1] = '\0';
	ft_strcat(after, line + pos);
	after[i + 1] = '\0';
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

void	history_search(int *i, char a)
{
	int		pos;

	if (a == 'u' && history->position <= history->nb_lines)
	{
		if (history->position == history->nb_lines)
			history->position--;
		pos = history->nb_lines - history->position - 1;
		free(g_data->line);
		g_data->line = ft_strdup(history->line[pos]);
		history->position++;
	}
	else if (a == 'd' && history->position >= 1)
	{
		if (history->position == 1)
		{
			history->position--;
			free(g_data->line);
			g_data->line = NULL;
		}
		else
		{
			history->position--;
			pos = history->nb_lines - history->position;
			free(g_data->line);
			g_data->line = ft_strdup(history->line[pos]);
		}
	}
	ft_rewrite(i);
}

void	print_key(char *buf)
{
	printf("\nbuf[0]: %d\n", buf[0]);
	printf("buf[1]: %d\n", buf[1]);
	printf("buf[2]: %d\n", buf[2]);
	printf("buf[3]: %d\n", buf[3]);
	printf("buf[4]: %d\n", buf[4]);
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

void	mode_icon(char icon)
{
	ft_put("sc");
	tputs(tgoto(tgetstr("ch", NULL), 0, g_data->cursor->start - 2), 0, my_outc);
	ft_putchar(icon);
	ft_put("rc");
}

void	ft_write(int pos, int *i)
{
	ft_clear(*i);
	ft_rewrite(i);
	while (g_data->pos > pos)
		ft_move('l', *i);
}

void	ft_sub(int len, int *i)
{
	while (len)
	{
		if (ft_move('l', *i))
			edit_line(i);
		len--;
	}
}

void	copy_mode(int *i)
{
	char	buf[20];
	int		select = 0;
	char	*cpy = ft_strdup("");
	char	*tmp;

	mode_icon('C');
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 20);
		if ((RIGHT||SPACE) && g_data->line)
		{
			if (g_data->line[g_data->pos])
			{
				if (!select)
				{
					ft_put("us");
					tmp = ft_charjoin(cpy, g_data->line[g_data->pos]);
					free(cpy);
					cpy = ft_strdup(tmp);
					free(tmp);
				}
				ft_putchar(g_data->line[g_data->pos]);
				ft_put("le");
				if (!ft_move('r', *i))
					ft_put("nd");
				ft_put("ue");
			}
		}
		else if (buf[0] == 'd')
		{
			mode_icon('P');
			ft_sub(ft_strlen(cpy), i);
			ft_write(g_data->pos, i);
			select = 1;
		}
		else if (buf[0] == 'y')
		{
			mode_icon('P');
			ft_write(g_data->pos, i);
			select = 1;
		}
		else if (buf[0] == 'p')
		{
			g_data->line = insert_str(g_data->line, cpy, g_data->pos, *i);
			select = 0;
			ft_clear(*i);
			*i = (int)ft_strlen(g_data->line);
			break ;
		}
		else if (LEFT || BACKSPACE)
		{
			if (cpy && !select)
				cpy[ft_strlen(cpy) - 1] = '\0';
			if (ft_move('l', *i))
			{
				ft_put("ue");
				ft_putchar(g_data->line[g_data->pos]);
				ft_put("le");
			}
		}
		else if (A_C || ECHAP)
		{
			if (select)
				ft_put("ue");
			break ;
		}
	}
	ft_rewrite(i);
	free(cpy);
	mode_icon(' ');
}

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
		else if (BACKSPACE)
		{
			if (ft_move('l', i))
				edit_line(&i);
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
			history->nb_lines ? history_search(&i, 'u') : 0;
		else if (A_RIGHT)
			word_right(i);
		else if (A_LEFT)
			word_left(i);
		else if (A_DOWN)
			line_down(i);
		else if (A_UP)
			line_up(i);
		else if (DOWN)
			history->nb_lines ? history_search(&i, 'd') : 0;
		else if (ECHAP)
			return (ft_strdup("exit"));
		else
			inser_char(buf[0], &i);
	}
	ft_put("ke");
	return (g_data->line);
}

void	environ_cpy(char **environ, char ***cpy)
{
	int i;

	i = 0;
	while (*(environ + i))
		i++;
	*cpy = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (*(environ + i))
	{
		(*(*cpy + i)) = ft_strdup(*(environ + i));
		i++;
	}
	*(*cpy + i) = NULL;
}

void	print_prompt(char **cpy)
{
	char *path;
	char *tmp;

	tmp = NULL;
	path = NULL;
	if (cpy)
		tmp = get_var(cpy, "PWD=");
	if (tmp)
	{
		path = ft_strrchr(tmp, '/');
		ft_strlen(path) > 1 ? path = path + 1 : 0;
		!path ? path = tmp : 0;
	}
	ft_putstr(ORANGE);
	ft_putchar('[');
	ft_put("us");
	ft_putstr("21sh");
	ft_put("ue");
	ft_putchar(']');
	ft_putstr(YELLOW);
	path ? ft_putstr(path) : 0;
	ft_putstr("$ ");
	ft_putstr(BLANK);
	g_data->cursor->start = ft_strlen(path) + 9;
	g_data->pos = 0;
	g_data->cursor->y = 0;
	g_data->cursor->x = 0;
}

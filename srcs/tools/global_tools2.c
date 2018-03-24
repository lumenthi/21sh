/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_tools2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:12:55 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/24 19:09:29 by lumenthi         ###   ########.fr       */
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

int		ft_move(char dir)
{
	if (dir == 'l')
	{
		if (g_data->cursor->x >= 21)
		{
			ft_put("le");
			g_data->cursor->x--;
			return (1);
		}
	}
	else if (dir == 'r')
	{
		if (g_data->cursor->x < ((int)ft_strlen(g_data->line) + 20))
		{
			ft_put("nd");
			g_data->cursor->x++;
			return (1);
		}
	}
	return (0);
}

char	*edit_line(char *line, int pos, int i)
{
	char	*before;
	char	*after;
	char	*all;

	line[i] = '\0';
	before = ft_strdup(line);
	after = ft_strdup(line);
	before[pos] = '\0';
	if (ft_strlen(before) == ft_strlen(line))
		return (before);
	after = after + (pos + 1);
	after[ft_strlen(line) - pos - 1] = '\0';
	all = ft_strjoin(before, after);
	return (all);
}

char	*gnl(void)
{
	char	buf[9];
	int		i;
	int		bu;

	g_data->line = NULL;
	i = 0;
	g_data->cursor->x = 20;
	ft_put("ks");
	while (1)
	{
		buf[0] = 0;
		buf[1] = 0;
		buf[2] = 0;
		read(0, buf, 9);
		if (buf[0] == 10)
		{
			ft_putchar('\n');
			g_data->cursor->x = 20;
			break ;
		}
		else if (buf[0] == 127)
		{
			if (ft_move('l'))
			{
				bu = g_data->cursor->x;
				ft_put("sc");
				ft_put("ce");
				g_data->line = edit_line(g_data->line, g_data->cursor->x - 20, i);
				while (ft_move('l'))
					;
				ft_putstr(g_data->line);
				g_data->cursor->x = bu;
				ft_put("rc");
				i--;
			}
		}
		else if (LEFT)
			ft_move('l');
		else if (RIGHT)
			ft_move('r');
		else if (ECHAP)
			return (ft_strdup("exit"));
		else
		{
			g_data->cursor->x++;
			ft_putchar(buf[0]);
			g_data->line = (char*)ft_realloc(g_data->line, i + 2);
			*(g_data->line + i) = buf[0];
			i++;
		}
	}
	ft_put("ke");
	if (i && g_data->line)
		*(g_data->line + i) = '\0';
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
	ft_putstr(BLUE);
	ft_putstr("[42minishell]");
	ft_putstr(BLANK);
	ft_putstr(GREEN);
	path ? ft_putstr(path) : 0;
	ft_putstr("$ ");
	ft_putstr(BLANK);
}

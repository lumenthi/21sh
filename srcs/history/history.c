/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 11:13:03 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:10:45 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static char		*read_file(void)
{
	int		fd;
	char	*line;
	char	buf[2];
	int		ret;

	fd = open(HISTORY_PATH, O_RDONLY);
	line = ft_strdup("");
	while ((ret = read(fd, buf, 1)))
	{
		buf[ret] = '\0';
		line = ft_strjoinl(line, buf);
	}
	close(fd);
	return (line);
}

void			get_lines(void)
{
	int		i;
	char	*bn;
	char	*line;
	char	*tmp;
	int		len;

	i = 0;
	line = read_file();
	tmp = ft_strdup(line);
	while ((bn = ft_strchr(tmp, '\n')))
	{
		*bn = '\0';
		g_history->line[i] = ft_strdup(tmp);
		len = ft_strlen(tmp) + 1;
		free(tmp);
		tmp = ft_strdup(line + len);
		free(line);
		line = ft_strdup(tmp);
		i++;
	}
	g_history->line[i] = NULL;
	free(line);
	free(tmp);
	g_history->nb_lines = i;
}

void			free_lines(void)
{
	int i;

	i = 0;
	if (g_history->nb_lines <= 0)
		return ;
	while (i <= g_history->nb_lines)
	{
		free(g_history->line[i]);
		i++;
	}
}

static int		line_char(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (ft_isalnum(line[i]) || line[i] == 34 || line[i] == 39 ||
			line[i] == '<' || line[i] == '>' || line[i] == '|' ||
			line[i] == ';')
			return (1);
		i++;
	}
	return (0);
}

void			write_file(void)
{
	int		fd;
	char	*tmp;

	fd = 0;
	if (!g_history->error && line_char(g_data->line) && !g_history->special)
	{
		free_lines();
		fd = history_open(fd);
		if (fd > 0)
		{
			tmp = ft_strjoin(g_data->line, "\n");
			write(fd, tmp, ft_strlen(tmp));
			free(tmp);
			get_lines();
			g_history->position = 0;
			close(fd);
		}
		else
			g_history->error = 1;
	}
	g_history->special = 0;
}

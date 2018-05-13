/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 11:13:03 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 11:21:09 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/21sh.h"

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

static void		get_lines(void)
{
	int		i;
	char	*bn;
	char	*line;
	char	*tmp;
	int		len;

	line = NULL;
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

void	free_lines(void)
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

static int		history_open(int fd)
{
	if (g_history->nb_lines < HISTORY_LIMIT)
		fd = open(HISTORY_PATH, O_RDWR|O_CREAT|O_APPEND, 0666);
	else
	{
		fd = open(HISTORY_PATH, O_RDWR|O_CREAT|O_TRUNC, 0666);
		ft_putstr(BLUE);
		ft_putstr("History limit reached, cleaning history...\n");
		ft_putstr(BLANK);
		g_history->nb_lines = 0;
	}
	return (fd);
}

void	history_init(void)
{
	int i = 0;
	int	fd;

	fd = 0;
	if (!(g_history = malloc(sizeof(t_history))))
		exit(-1);
	if (!(g_history->line = malloc(sizeof(char *) * HISTORY_LIMIT + 1)))
		exit(-1);
	g_history->nb_lines = 0;
	g_history->error = 0;
	g_history->special = 0;
	fd = history_open(fd);
	if (HISTORY_LIMIT < 0)
	{
		history_error();
		g_history->position = 0;
		return ;
	}
	if (fd > 0)
	{
		while (i <= HISTORY_LIMIT)
		{
			g_history->line[i] = NULL;
			i++;
		}
		get_lines();
		close(fd);
	}
	else if (fd < 0)
		history_error();
	g_history->position = 0;
}

void	history_free(void)
{
	free_lines();
	free(g_history->line);
	free(g_history);
}

static int	line_char(char *line)
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

void	write_file(void)
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

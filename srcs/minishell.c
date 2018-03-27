/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/27 19:05:15 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	inthandler(int sig)
{
	if (sig == 2)
		g_data->error = 1;
	ft_putchar('\n');
	print_prompt(g_data->cpy);
}

static void	ft_apply(char **line, char **args)
{
	if (args[0] && ft_strcmp(args[0], "echo") == 0)
		ft_echo(args, g_data->cpy);
	else if (args[0] && ft_strcmp(args[0], "cd") == 0)
		ft_cd(&g_data->cpy, args);
	else if (args[0] && ft_strcmp(args[0], "setenv") == 0)
		ft_setenv(&g_data->cpy, args);
	else if (args[0] && ft_strcmp(args[0], "unsetenv") == 0)
		ft_unsetenv(&g_data->cpy, args);
	else if (args[0] && ft_strcmp(args[0], "env") == 0)
		ft_env(&g_data->cpy, args, line);
	else
		ft_execve(args, g_data->cpy);
}

int			ft_minishell(char **line)
{
	char **args;

	args = NULL;
	args = get_a(*line, args);
	if (!args)
		ft_print_error(NULL, QUOTES, *line);
	else if (args[0] &&
		(ft_strcmp(args[0], "exit") == 0 || ft_strcmp(args[0], "q") == 0))
	{
		ft_tabdel(&args);
		free(args);
		return (1);
	}
	else
		ft_apply(line, args);
	free(*line);
	if (args)
	{
		ft_tabdel(&args);
		free(args);
	}
	return (0);
}

static void	data_init(void)
{
	g_data = (t_data *)malloc(sizeof(t_data));
	g_data->cursor = (t_cursor *)malloc(sizeof(t_cursor));
	g_data->cursor->x = 0;
	g_data->cursor->y = 0;
}

static void	flag_init(struct termios *term)
{
	if (term->c_lflag == ISIG)
		term->c_lflag &= ~(ISIG);
	if (term->c_lflag != ICANON)
		term->c_lflag &= ~(ICANON);
	if (term->c_lflag != ECHO)
		term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
}

static void	term_init(void)
{
	char	*name_term;
	struct	termios term;

	g_data->bu = malloc(sizeof(struct termios));
	name_term = getenv("TERM");
	tgetent(NULL, name_term);
	tcgetattr(0, g_data->bu);
	tcgetattr(0, &term);
	flag_init(&term);
	tcsetattr(0, TCSADRAIN, &term);
}

#include <stdio.h>

static void		get_lines(void)
{
	int		fd;
	char	*line = NULL;
	int		i;

	i = 0;
	fd = open(".21sh_history", O_RDONLY|O_CREAT, 0666);
	while (get_next_line(fd, &line))
	{
		history->line[i] = ft_strdup(line);
		free(line);
		i++;
	}
	history->line[i] = NULL;
	free(line);
	close(fd);
	history->nb_lines = i;
}

static void	free_lines(void)
{
	int i;

	i = 0;
	while (i <= history->nb_lines)
	{
		free(history->line[i]);
		i++;
	}
}

static int	history_open(void)
{
	if (history->nb_lines < HISTORY_LIMIT)
	{
		history->fd = open(".21sh_history", O_RDWR|O_CREAT|O_APPEND, 0666);
		return (1);
	}
	else
	{
		history->fd = open(".21sh_history", O_RDWR|O_CREAT|O_TRUNC, 0666);
		ft_putstr(BLUE);
		ft_putstr("History limit reached, cleaning history...\n");
		ft_putstr(BLANK);
		history->nb_lines = 0;
		return (0);
	}
}

static void	history_init(void)
{
	history = malloc(sizeof(t_history));
	history_open();
	get_lines();
	history->position = 0;
}

int			main(void)
{
	extern char	**environ;
	char		*line;

	history_init();
	data_init();
	term_init();
	get_winsize();
	g_data->error = 0;
	environ_cpy(environ, &g_data->cpy);
	signal(SIGINT, inthandler);
	while (1)
	{
		line = NULL;
		if (!g_data->error)
			print_prompt(g_data->cpy);
		line = gnl();
		g_data->error = 0;
		if (line)
		{
			if (line[0] != '\0')
			{
				history_open();
				write(history->fd, line, ft_strlen(line));
				write(history->fd, "\n", 1);
				free_lines();
				get_lines();
				history->position = 0;
			}
			if (ft_minishell(&line))
				break ;
		}
	}
	tcsetattr(0, 0, g_data->bu);
	ft_tabdel(&g_data->cpy);
	free(g_data->cpy);
	free(g_data->bu);
	free(g_data);
	free(line);
	close(history->fd);
	return (0);
}

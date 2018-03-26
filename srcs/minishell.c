/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/26 23:21:47 by lumenthi         ###   ########.fr       */
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

static void	history_init(void)
{
	history = malloc(sizeof(t_history));
	history->fd = open(".21sh_history", O_RDWR|O_CREAT|O_TRUNC, 0666);
	history->position = 0;
	history->nb_lines = 0;
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
				write(history->fd, line, ft_strlen(line));
				write(history->fd, "\n", 1);
				history->nb_lines++;
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
	return (0);
}

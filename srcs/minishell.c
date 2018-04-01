/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/31 16:25:10 by lumenthi         ###   ########.fr       */
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

void	ft_history(char **args)
{
	int i;
	(void)args;

	i = 0;
	ft_putendl("history:\n");
	while (history->line[i + 1])
	{
		ft_putnbr(i + 1);
		ft_putstr(" - [");
		ft_putstr(history->line[i]);
		ft_putendl("]");
		i++;
	}
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
	else if (args[0] && ft_strcmp(args[0], "history") == 0)
		ft_history(args);
	else
		ft_execve(args, g_data->cpy);
}

int			ft_minishell(char **line)
{
	char **args;

	args = NULL;
//	get_a("test", args);
	args = ft_strsplit(*line, ' ');
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
	if (args)
	{
		ft_tabdel(&args);
		free(args);
	}
	free(*line);
	return (0);
}

static void	data_free(void)
{
	ft_tabdel(&g_data->cpy);
	free(g_data->cpy);
	free(g_data->cursor);
	free(g_data->bu);
	free(g_data->line);
	free(g_data);
}

static void	data_init(void)
{
	g_data = (t_data *)malloc(sizeof(t_data));
	g_data->cursor = (t_cursor *)malloc(sizeof(t_cursor));
	g_data->pos = 0;
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

static char		*read_file(void)
{
	int		fd;
	char	*line;
	char	*tmp;
	char	buf[2];
	int		ret;

	fd = open("/tmp/.21sh_history", O_RDONLY);
	line = ft_strdup("");
	while ((ret = read(fd, buf, 1)))
	{
		buf[ret] = '\0';
		tmp = ft_strdup(line);
		free(line);
		line = ft_strjoin(tmp, buf);
		free(tmp);
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
		history->line[i] = ft_strdup(tmp);
		len = ft_strlen(tmp) + 1;
		free(tmp);
		tmp = ft_strdup(line + len);
		free(line);
		line = ft_strdup(tmp);
		i++;
	}
	history->line[i] = NULL;
	free(line);
	free(tmp);
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

static int	history_open(int fd)
{
	if (history->nb_lines < HISTORY_LIMIT)
		fd = open("/tmp/.21sh_history", O_RDWR|O_CREAT|O_APPEND, 0666);
	else
	{
		fd = open("/tmp/.21sh_history", O_RDWR|O_CREAT|O_TRUNC, 0666);
		ft_putstr(BLUE);
		ft_putstr("History limit reached, cleaning history...\n");
		ft_putstr(BLANK);
		history->nb_lines = 0;
	}
	return (fd);
}

static void	history_init(void)
{
	int i = 0;
	int	fd;

	fd = 0;
	history = malloc(sizeof(t_history));
	history->line = malloc(sizeof(char *) * HISTORY_LIMIT + 1);
	history->nb_lines = 0;
	fd = history_open(fd);
	while (i <= HISTORY_LIMIT)
	{
		history->line[i] = NULL;
		i++;
	}
	get_lines();
	close(fd);
	history->position = 0;
}

static void	history_free(void)
{
	free_lines();
	free(history->line);
	free(history);
}

static int	line_char(char *line)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (ft_isalnum(line[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	write_file(void)
{
	int fd;
	char	*tmp;

	fd = 0;
	if (line_char(g_data->line))
	{
		free_lines();
		fd = history_open(fd);
		tmp = ft_strjoin(g_data->line, "\n");
		write(fd, tmp, ft_strlen(tmp));
		free(tmp);
		get_lines();
		history->position = 0;
		close(fd);
	}
}

int			main(void)
{
	extern char	**environ;

	history_init();
	data_init();
	term_init();
	get_winsize();
	g_data->error = 0;
	environ_cpy(environ, &g_data->cpy);
	signal(SIGINT, inthandler);
	while (1)
	{
		g_data->line = NULL;
		if (!g_data->error)
			print_prompt(g_data->cpy);
		g_data->line = gnl();
		g_data->error = 0;
		if (g_data->line)
		{
			write_file();
			if (ft_minishell(&g_data->line))
				break ;
		}
	}
	tcsetattr(0, 0, g_data->bu);
	data_free();
	history_free();
	return (0);
}

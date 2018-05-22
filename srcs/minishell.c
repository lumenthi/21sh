/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/22 20:18:58 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

void	dup_std(void)
{
	if (g_input->std0 != 0)
		dup2(g_input->std0, 0);
	if (g_input->std1 != 0)
		dup2(g_input->std1, 1);
	if (g_input->std2 != 0)
		dup2(g_input->std2, 2);
}

static void	retab_init(char ***args, int *j, char **tmp, int i)
{
	*j = 0;
	while (*j <= i)
	{
		*tmp = ft_strdup(*(*args + *j));
		free(*(*args + *j));
		*(*args + *j) = ft_strdup(*tmp);
		free(*tmp);
		(*j)++;
	}
}

void	ft_retab(char **args, int i)
{
	int		j;
	char	*tmp;

	retab_init(&args, &j, &tmp, i);
	while (args[i])
	{
		if (args[i + 1])
		{
			free(args[i]);
			args[i] = ft_strdup(args[i + 1]);
		}
		else
		{
			free(args[i]);
			args[i] = NULL;
			break ;
		}
		i++;
	}
}

static void	inputs_init()
{
	if (!(g_input = malloc(sizeof(g_input))))
		exit(-1);
	g_input->std0 = 0;
	g_input->std1 = 0;
	g_input->std2 = 0;
	g_input->op = 0;
}

static void	inputs_reset()
{
	free(g_input);
}

static void	just_apply2(char ***args, int *fd)
{
	if (*(*args + 0) && ft_strcmp(*(*args + 0), "echo") == 0)
		ft_echo(*args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "cd") == 0)
		ft_cd(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "setenv") == 0)
		ft_setenv(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "unsetenv") == 0)
		ft_unsetenv(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "env") == 0)
		ft_env(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "history") == 0)
		ft_history(*args);
	else
		ft_execve(*args, g_data->cpy);
	if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
}

void	just_apply(char ***args)
{
	int i;
	int fd;

	i = 0;
	if ((fd = ft_redir(args)) == -1)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		return ;
	}
	while (*(*args + i))
	{
		*(*args + i) = args_translate(*(*args + i), *args);
		i++;
	}
	if (*(*args + 0) && ft_strcmp(*(*args + 0), "exit") == 0)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		return ;
	}
	just_apply2(args, &fd);
}

int		count_pipes(char **args)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			count = count + 2;
		i++;
	}
	return(count);
}

static t_nrm	*apply_init(int tube[], int **o_pid, char ***args, char ***arg)
{
	t_nrm *nrm;

	if (!(nrm = (t_nrm *)malloc(sizeof(t_nrm))))
		exit(-1);
	if (!(*o_pid = (int *)malloc(sizeof(int) * count_pipes(*arg))))
		exit(-1);
	if (!(*args = (char **)malloc(sizeof(char *) * tab_size(*arg) + 1)))
		exit(-1);
	nrm->count = 0;
	pipe_pid = 0;
	nrm->i = 0;
	nrm->j = 0;
	tube[0] = 0;
	tube[1] = 0;
	nrm->std = dup(0);
	nrm->std1 = dup(1);
	return (nrm);
}

static void		process_pipe(char ***args)
{
	int		tunnel[2];

	pipe(tunnel);
	if (!fork())
	{
		close(tunnel[0]);
		in_pipe = 1;
		just_apply(args);
		dup_std();
		write(tunnel[1], &pipe_pid, sizeof(int));
		close(tunnel[1]);
		exit(0);
	}
	else
	{
		close(tunnel[1]);
		wait(NULL);
		read(tunnel[0], &pipe_pid, sizeof(int));
		close(tunnel[0]);
	}
}

static void		do_pipes(int tube[], t_nrm **nrm, char ***args, int **old_pids)
{
	if (ft_strcmp((*args)[(*nrm)->j], "|") == 0)
	{
		pipe(tube);
		dup2(tube[1], 1);
		free((*args)[(*nrm)->j]);
		if (pipe_pid != 0)
		{
			(*old_pids)[(*nrm)->count] = pipe_pid;
			(*nrm)->count++;
			pipe_pid = 0;
		}
		(*args)[(*nrm)->j] = NULL;
		process_pipe(args);
		ft_tabdel(args);
		(*nrm)->j = -1;
		dup2(tube[0], 0);
		close(tube[1]);
	}
}

static void		end_apply(char ***args, int **old_pids, t_nrm **nrm)
{
	dup2((*nrm)->std1, 1);
	in_pipe = 0;
	just_apply(args);
	dup_std();
	(*args)[tab_size(*args)] = NULL;
	(*old_pids)[(*nrm)->count] = 0;
	(*nrm)->count = 0;
	while ((*old_pids)[(*nrm)->count] != 0)
	{
		kill((*old_pids)[(*nrm)->count], SIGTERM);
		(*nrm)->count++;
	}
	dup2((*nrm)->std, 0);
	free(*nrm);
	free(*old_pids);
	ft_tabdel(args);
	free(*args);
}

void			ft_apply(char **arg)
{
	int		tube[2];
	char	**args;
	int		*old_pids;
	t_nrm	*nrm;

	nrm = apply_init(tube, &old_pids, &args, &arg);
	while (arg[nrm->i])
	{
		args[nrm->j] = ft_strdup(arg[nrm->i]);
		do_pipes(tube, &nrm, &args, &old_pids);
		nrm->j++;
		nrm->i++;
	}
	args[nrm->j] = NULL;
	if (pipe_pid != 0)
	{
		old_pids[nrm->count] = pipe_pid;
		nrm->count++;
		pipe_pid = 0;
	}
	end_apply(&args, &old_pids, &nrm);
}

static void	flag_init(struct termios *term)
{
	term->c_lflag &= ~(ISIG);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
}

void	termios_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": cant initialize struct termios\n", 2);
}

int		valid_term(void)
{
	char	*name_term;

	name_term = get_var(g_data->cpy, "TERM=");
	if (!name_term)
	{
		termios_error();
		return (0);
	}
	return (1);
}

void	term_init(void)
{
	char	*name_term;
	struct	termios term;

	if (!(g_data->bu = malloc(sizeof(struct termios))))
		exit(-1);
	name_term = get_var(g_data->cpy, "TERM=");
	tgetent(NULL, name_term);
	tcgetattr(0, g_data->bu);
	tcgetattr(0, &term);
	flag_init(&term);
	tcsetattr(0, TCSADRAIN, &term);
}

void		term_reset()
{
	if (g_data->bu != NULL)
	{
		tcsetattr(0, 0, g_data->bu);
		free(g_data->bu);
		g_data->bu = NULL;
	}
}

int			ft_minishell(char **line)
{
	char	**args;

	args = NULL;
	term_init();
	quote_get2(line);
	term_reset();
	args = get_a(*line, args);
	ft_apply(args);
	if (args[0] && ft_strcmp(args[0], "exit") == 0)
		return (1);
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
	free(g_data->line);
	free(g_data);
}

static void	data_init(void)
{
	if (!(g_data = (t_data *)malloc(sizeof(t_data))))
		exit(-1);
	if (!(g_data->cursor = (t_cursor *)malloc(sizeof(t_cursor))))
		exit(-1);
	g_data->pos = 0;
	g_data->cursor->x = 0;
	g_data->cursor->y = 0;
}

char	*strchr_quote(char *line, int elem)
{
	int		i;
	char	*found;
	int		mode;

	mode = 0;
	i = 0;
	found = NULL;
	while (line[i])
	{
		if (line[i] == 34 && mode != 39)
			mode = mode ? 0 : 34;
		if (line[i] == 39 && mode != 34)
			mode = mode ? 0 : 39;
		if (line[i] == elem && !mode)
			return (found = line + i);
		i++;
	}
	return (NULL);
}

static int		end_commands(char **str, char **base, char **line)
{
	if (ft_minishell(str))
	{
		free(*base);
		free(*str);
		return (1);
	}
	free(*line);
	free(*base);
	return (0);
}

static void		loop_init(char **found, char **str, char **base, char **tmp)
{
	*(*found + 1) = '\0';
	*(*str + ft_strlen(*str) - 1) = '\0';
	*tmp = ft_strdup(*base);
	free(*base);
	*base = ft_strdup(*tmp + ft_strlen(*str) + 1);
}

static int		do_minishell(char **str, char **base, char **tmp)
{
	if (ft_minishell(str))
	{
		free(*base);
		free(*str);
		return (1);
	}
	free(*tmp);
	return (0);
}

int				ft_commands(char **line)
{
	char	*str;
	char	*found;
	char	*base;
	char	*tmp;

	base = ft_strdup(*line);
	str = ft_strdup(*line);
	while ((found = strchr_quote(str, ';')))
	{
		loop_init(&found, &str, &base, &tmp);
		if (do_minishell(&str, &base, &tmp))
			return (1);
		if (g_data->error)
		{
			free(*line);
			free(base);
			return (0);
		}
		str = ft_strdup(base);
	}
	return (end_commands(&str, &base, line));
}

void		set_lvl(void)
{
	int		nb;
	char	*line;

	line = get_var(g_data->cpy, "SHLVL=");
	if (line)
	{
		nb = ft_atoi(line) + 1;
		line = ft_itoa(nb);
	}
	else
		line = ft_strdup("1");
	set_var(&g_data->cpy, "SHLVL=", line);
	free(line);
}

static int	ft_shell(void)
{
	g_data->line = NULL;
	get_winsize();
	term_init();
	if (!g_data->error)
		print_prompt(g_data->cpy);
	g_data->line = gnl();
	term_reset();
	g_data->error = 0;
	if (g_data->line)
	{
		write_file();
		if (ft_commands(&g_data->line))
			return (1);
	}
	return (0);
}

int			main(void)
{
	extern char	**environ;

	history_init();
	data_init();
	inputs_init();
	g_data->error = 0;
	environ_cpy(environ, &g_data->cpy);
	all_signals();
	valid_term();
	set_lvl();
	while (1)
	{
		if (ft_shell())
			break ;
	}
	inputs_reset();
	data_free();
	history_free();
	return (0);
}

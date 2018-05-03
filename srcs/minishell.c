/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/03 14:55:03 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/21sh.h"

static void	dup_std()
{
	if (g_input->std0 != 0)
		dup2(g_input->std0, 0);
	if (g_input->std1 != 0)
		dup2(g_input->std1, 1);
	if (g_input->std2 != 0)
		dup2(g_input->std2, 2);
}

static void	inthandler(int sig)
{
	if (sig == 2)
		g_data->error = 1;
	dup_std();
	ft_putstr("\b ");
	ft_putstr("\b\b ");
	ft_putstr("\b");
	ft_putchar('\n');
	print_prompt(g_data->cpy);
	if (g_data->line)
		free(g_data->line);
	g_data->line = ft_strdup("");
}

static void	history_free(void);
static void	free_lines(void);
static void	history_error(void);

void	ft_history(char **args)
{
	int	i;
	int	fd;

	i = 0;
	if (tab_size(args) == 1)
	{
		ft_putendl("history:\n");
		if (g_history->nb_lines <= 0)
			return ;
		while (g_history->line[i + 1])
		{
			ft_putnbr(i + 1);
			ft_putstr(" - [");
			ft_putstr(g_history->line[i]);
			ft_putendl("]");
			i++;
		}
	}
	else if (ft_strcmp(args[1], "clean") == 0 ||
		ft_strcmp(args[1], "clear") == 0 || ft_strcmp(args[1], "reset") == 0)
	{
		fd = open(HISTORY_PATH, O_RDWR|O_CREAT|O_TRUNC, 0666);
		if (fd < 0)
		{
			g_history->error = 1;
			history_error();
			return ;
		}
		else
			g_history->error = 0;
		free_lines();
		ft_putstr(GREEN);
		ft_putstr("Cleaning history...");
		ft_putstr(BLANK);
		ft_putchar('\n');
		g_history->nb_lines = 0;
		close(fd);
	}
}

static void	ft_retab(char **args, int i)
{
	int	j = 0;
	char	*tmp;

	while (j <= i)
	{
		tmp = ft_strdup(args[j]);
		free(args[j]);
		args[j] = ft_strdup(tmp);
		free(tmp);
		j++;
	}
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
/*
char	*get_content(int fd)
{
	char	*line;
	char	buf[2];
	int		ret;

	line = ft_strdup("");
	while ((ret = read(fd, buf, 1)))
	{
		buf[ret] = '\0';
		line = ft_strjoinl(line, buf);
	}
	return (line);
}
*/
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

static void	input_error(char *line)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(line, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": no such file or directory\n", 2);
}

void		parse_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": parse error\n", 2);
}

int		ft_isnum(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	fd_error(char *str)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": not a valid fd\n", 2);
}

void	last_redir(char **after)
{
	char *found;

	found = NULL;
	if (!(found = ft_strrchr(*after, '>')))
		found = ft_strrchr(*after, '<');
	if (!found)
		;
	else
	{
		free(*after);
		*after = ft_strdup(found + 1);
	}
//	dprintf(2, "after: |%s|\n", after);
}

static int	is_redir(char *line)
{
	char	*found;
	char	*after;
	char	*cpy;
	int		new_fd;
	int		sign;

	new_fd = 0;
	cpy = ft_strdup(line);
//	dprintf(2, "cpy: |%s|\n", cpy);
	if (ft_strcmp(line, ">") == 0 || ft_strcmp(line, "<") == 0 ||
	ft_strcmp(line, "<<") == 0 || ft_strcmp(line, ">>") == 0)
	{
		free(cpy);
		return (-1);
	}
	if ((found = ft_strchr(cpy, '<'))
	|| (found = ft_strchr(cpy, '>')))
	{
//		ft_putstr("IN");
		if (*(found + 1) == '<' || *(found + 1) == '>')
		{
			after = ft_strdup(found + 2);
			sign = 2;
		}
		else
		{
			after = ft_strdup(found + 1);
			sign = 1;
		}
		*found = '\0';
//		last_redir(&after);
//		dprintf(2, "after: |%s|\n", after);
		if (ft_strchr(after, '&'))
		{
			if (ft_strcmp(after, "&-") == 0)
				new_fd = open("/dev/null", O_RDONLY);
			else if (ft_strcmp(after, "&0") == 0 || ft_strcmp(after, "&1") == 0
				|| ft_strcmp(after, "&2") == 0)
				new_fd = ft_atoi(after + 1);
			else
			{
				if (sign == 1)
					new_fd = open(after + 1, O_RDWR);
				else
					new_fd = open(after + 1, O_RDWR|O_APPEND);
			}
		}
		else
		{
			if (sign == 1)
				new_fd = open(after, O_RDWR|O_CREAT|O_TRUNC, 0666);
			else
				new_fd = open(after, O_RDWR|O_CREAT|O_APPEND, 0666);
		}
		if (new_fd < 0)
		{
			fd_error(after + 1);
			free(after);
			free(cpy);
			return (-2);
		}
		if (ft_isnum(cpy))
		{
//			ft_putstr_fd("IN", 2);
//			dprintf(2, "cpy: |%s|\n", cpy);
//			dprintf(2, "cpy: |%d|\nnew_fd: |%d|\n", ft_atoi(cpy), new_fd);
			if (ft_atoi(cpy) == 0)
				g_input->std0 = dup(ft_atoi(cpy));
			else if (ft_atoi(cpy) == 1)
				g_input->std1 = dup(ft_atoi(cpy));
			else if (ft_atoi(cpy) == 2)
				g_input->std2 = dup(ft_atoi(cpy));
			dup2(new_fd, ft_atoi(cpy));
			g_input->op = 1;
		}
		free(after);
		free(cpy);
		return (new_fd);
	}
	free(cpy);
	return (-1);
}

static int	first_redir(char **args)
{
	int i;
	int	ret;
	int	fd;

	fd = 0;
	ret = 0;
	i = 0;
//	ft_printtab(args);
	while (args[i])
	{
		if (args[i][0] == 39 || args[i][0] == 34)
			;
		else if (ft_strcmp(args[i], ">>") != 0 && (ft_strcmp(args[i], "<<") != 0)
			&& ((ret = is_redir(args[i])) >= 0) && ft_strlen(args[i]) != 1)
		{
//			ft_putstr_fd("IN", 2);
			fd = ret;
			ft_retab(args, i);
			i--;
		}
		if (ret == -2)
			return (-1);
		i++;
	}
	return (fd);
}

static char	**tab_insert(char **args, int i, char *new)
{
	int	size;
	int	c;

	size = tab_size(args);
	size++;
	c = size;
	args = (char**)ft_realloc(args, sizeof(char *) * (size + 1));
	while (c > i)
	{
		args[c] = args[c - 1];
		c--;
	}
	args[c] = ft_strdup(new);
	args[size] = NULL;
	return (args);
}

static char	**tab_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	if (*(found + 1) == '<' || *(found + 1) == '>')
	{
		after = ft_strdup(found + 2);
		sign = ft_strdup(found);
		sign[2] = '\0';
	}
	else
	{
		after = ft_strdup(found + 1);
		sign = ft_strdup(found);
		sign[1] = '\0';
	}
	*found = '\0';
//	dprintf(2, "before: |%s|\n", before);
//	dprintf(2, "sign: |%s|\n", sign);
//	dprintf(2, "after: |%s|\n", after);
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	args = tab_insert(args, i + 2, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char **before_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

//	dprintf(2, "args[i]: |%s|\n", args[i]);
	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	if (*(found + 1) == '<' || *(found + 1) == '>')
	{
		after = ft_strdup(found + 2);
		sign = ft_strdup(found);
		sign[2] = '\0';
	}
	else
	{
		after = ft_strdup(found + 1);
		sign = ft_strdup(found);
		sign[1] = '\0';
	}
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(sign);
	args = tab_insert(args, i + 1, after);
//	args = tab_insert(args, i + 2, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char **after_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

//	dprintf(2, "args[i]: |%s|\n", args[i]);
	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	if (*(found + 1) == '<' || *(found + 1) == '>')
	{
		after = ft_strdup(found + 2);
		sign = ft_strdup(found);
		sign[2] = '\0';
	}
	else
	{
		after = ft_strdup(found + 1);
		sign = ft_strdup(found);
		sign[1] = '\0';
	}
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char **nb_lastredir(char **args, int i)
{
	char	*found;
	char	*line;
	char	*before;
//	char	*last;

	line = ft_strdup(args[i]);
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	if (!found)
		return (args);
	*found = '\0';
//	dprintf(2, "before: |%s|\n", before);
	if (!(found = ft_strrchr(line, '>')))
		found = ft_strrchr(line, '<');
//	dprintf(2, "found: |%s|\n", found);
	free(args[i]);
	args[i] = ft_strjoin(before, found);
	free(line);
	free(before);
//	dprintf(2, "args[i]: |%s|\n", args[i]);
//	ft_printtab(args);
	return (args);
}

static int	opin_quote(char *str)
{
	int		i;
	int		mode;

	i = 0;
	mode = 0;
	while (str[i])
	{
		if (str[i] == 34 && mode != 39)
		{
			if (mode)
				mode = 0;
			else if (!mode)
				mode = 34;
		}
		else if (str[i] == 39 && mode != 34)
		{
			if (mode)
				mode = 0;
			else if (!mode)
				mode = 39;
		}
		if ((str[i] == '>' || str[i] == '<') && mode != 0)
			return (1);
		else if ((str[i] == '>' || str[i] == '<') && mode == 0)
			return (0);
		i++;
	}
	return (0);
}

char	*strchr_quote(char *line, int elem);

static int	valid_redir(char *str)
{
	char	*found;
	int		i;
	int		j;
	int		other;
	char	*tmp;

	i = 0;
	j = 0;
	found = strchr_quote(str, '>');
	tmp = strchr_quote(str, '<');
	if (!found && !tmp)
		return (1);
	else
	{
		if (ft_strlen(found) < ft_strlen(tmp))
			found = tmp;
		if (found[0] == '<')
			other = '>';
		else if (found[0] == '>')
			other = '<';
		while (found[j] == found[0])
			j++;
		if (j > 2 || found[j] == '|' || found[j] == ';' || found[j] == other)
			return (0);
		if (!valid_redir(found + j))
			return (0);
	}
	return (1);
}

static int	arg_last_redir(char **str)
{
	char	*found;
	char	*after;
	char	*cpy;

	if (!(valid_redir(*str)))
	{
		parse_error();
		return (-1);
	}
	cpy = ft_strdup(*str);
	if (!(found = ft_strrchr(cpy, '<')))
		found = ft_strrchr(cpy, '>');
	if (*(found - 1) == '>')
		found = found - 1;
	after = ft_strdup(found);
	if (!(found = ft_strchr(cpy, '<')))
		found = ft_strchr(cpy, '>');
	*found = '\0';
	free(*str);
	*str = ft_strjoin(cpy, after);
	if (ft_strchr(after, '&'))
	{
		free(after);
		free(cpy);
		return (0);
	}
	free(after);
	free(cpy);
	return (1);
}
static char	**retab_dirs(char **args)
{
	int		i;
	char	*after;
	char	*found;
	char	*cpy;
	int		ret;

	i = 0;
	ret = 0;
	while (args[i])
	{
		
		cpy = ft_strdup(args[i]);
		if (opin_quote(cpy))
			;
		else if (((found = ft_strchr(cpy, '>')) ||
			(found = ft_strchr(cpy, '<'))) &&
			ft_strcmp(cpy, ">>") != 0 && (ft_strcmp(cpy, "<<") != 0) &&
			ft_strlen(args[i]) != 1)
		{
//			dprintf(2, "1 - args[i]: |%s|\n", args[i]);
			if ((ret = arg_last_redir(&args[i])) == -1)
			{
				free(cpy);
				return (NULL);
			}
			else if (ret == 0)
				;
			else
			{
				after = ft_strdup(found + 1);
				*found = '\0';
//				dprintf(2, "cpy: |%s|\n", cpy);
//				dprintf(2, "after: |%s|\n", after);
				if (ft_isnum(cpy) && ft_strcmp(cpy, "") != 0)
					args = nb_lastredir(args, i);
				else if (ft_strcmp(cpy, "") == 0)
					args = before_resize(args, i);
				else if (ft_strcmp(after, "") == 0 || ft_strcmp(after, ">") == 0 ||
					ft_strcmp(after, "<") == 0)
					args = after_resize(args, i);
				else
					args = tab_resize(args, i);
				free(after);
			}
		}
		free(cpy);
		i++;
	}
//	ft_printtab(args);
	return (args);
}

static char	**before_pipes(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	found = ft_strchr(before, '|');
	after = ft_strdup(found + 1);
	sign = ft_strdup(found);
	sign[1] = '\0';
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(sign);
	args = tab_insert(args, i + 1, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char **after_pipes(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	found = ft_strchr(before, '|');
	after = ft_strdup(found + 1);
	sign = ft_strdup(found);
	sign[1] = '\0';
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char **resize_pipes(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	found = ft_strchr(before, '|');
	if (*(found + 1) == '|')
	{
		parse_error();
		return (NULL);
	}
	after = ft_strdup(found + 1);
	sign = ft_strdup(found);
	sign[1] = '\0';
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	args = tab_insert(args, i + 2, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static char	**retab_pipes(char **args)
{
	int		i;
	char	*after;
	char	*found;
	char	*cpy;

	i = 0;
	while (args[i])
	{
		cpy = ft_strdup(args[i]);
		if ((found = strchr_quote(cpy, '|')) && ft_strlen(args[i]) != 1)
		{
			if (*(found + 1) == '|' || *(found + 1) == '<' ||
				*(found + 1) == '>')
			{
				parse_error();
				free(cpy);
				return (NULL);
			}
			if (ft_strlen(args[i]) != ft_strlen(found))
			{
				if (*(found - 1) == '<' || *(found - 1) == '>')
				{
					parse_error();
					free(cpy);
					return (NULL);
				}
			}
			after = ft_strdup(found + 1);
			*found = '\0';
			if (ft_strcmp(cpy, "") == 0)
				args = before_pipes(args, i);
			else if (ft_strcmp(after, "") == 0)
				args = after_pipes(args, i);
			else
				args = resize_pipes(args, i);
			free(after);
		}
		free(cpy);
		i++;
	}
	return (args);
}

static int	ft_redir(char ***arg)
{
	int		i;
	int		fd;
	int		tube[2];
	char	**args;

	tube[0] = 0;
	tube[1] = 0;
	g_input->op = 0;
	g_input->std0 = 0;
	g_input->std1 = 0;
	g_input->std2 = 0;
	fd = 0;
	i = 0;
	if (!(args = retab_pipes(*arg)))
		return (-1);
//	ft_printtab(*arg);
	if (!(args = retab_dirs(args)))
		return (-1);
	else
		*arg = args;
//	ft_printtab(args);
	fd = first_redir(args);
//	ft_putnbr(fd);
//	if (fd == 1 || fd == 2 || fd == 0)
//		fd = 3;
	while (args[i])
	{
//		dprintf(2, "args[i]: %s\n", args[i]);
//		if (!(valid_redir(args[i])))
//			return (-1);
		if (ft_strcmp(args[i], "<<") == 0)
		{
			if (!args[i + 1])
			{
				parse_error();
				return (-1);
			}
			if (tube[0] != 0)
			{
				ft_retab(args, i + 1);
				ft_retab(args, i);
			}
			else
			{
//				dprintf(2, "before pipe | tube[1]: %d, tube[0]: %d\n", tube[1], tube[0]);
				pipe(tube);
//				dprintf(2, "after pipe | tube[1]: %d, tube[0]: %d\n", tube[1], tube[0]);
				term_init();
				write_mode(tube[1], args[i + 1]);
				term_reset();
				ft_retab(args, i + 1);
				ft_retab(args, i);
//				dprintf(2, "closing fd[%d]\n", tube[1]);
				close(tube[1]);
			}
		}
		if (g_data->error == 1)
		{
			print_prompt(g_data->cpy);
			return (-1);
		}
		i++;
	}
	i = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], "<") == 0 && args[i + 1]) || tube[0] != 0)
		{
			if (args[i + 1])
				args[i + 1] = args_translate(args[i + 1]);
			if (fd > 0 && fd != 0 && fd != 1 && fd != 2)
				close(fd);
			if (tube[0] != 0)
				fd = tube[0];
			else
			{
				if ((fd = open(args[i + 1], O_RDWR)) < 0)
				{
					input_error(args[i + 1]);
					if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
						close(fd);
					dup_std();
					return (fd);
				}
				ft_retab(args, i + 1);
				ft_retab(args, i);
				i--;
			}
			if (g_input->std0 != 0)
				dup2(g_input->std0, 0);
			g_input->std0 = dup(0);
//			printf("fd: %d\n", fd);
			dup2(fd, 0);
			g_input->op = 1;
			if (tube[0] != 0)
			{
				close(tube[0]);
				tube[0] = 0;
			}
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1] && i != 0)
		{
			args[i + 1] = args_translate(args[i + 1]);
			if (fd > 0 && fd != 0 && fd != 1 && fd != 2)
				close(fd);
			if ((fd = open(args[i + 1], O_RDWR|O_CREAT|O_APPEND, 0666)) < 0)
			{
				input_error(args[i + 1]);
				if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
					close(fd);
				dup_std();
				return (fd);
			}
			else
			{
				ft_retab(args, i + 1);
				ft_retab(args, i);
				i--;
				if (g_input->std1 != 0)
					dup2(g_input->std1, 1);
				g_input->std1 = dup(1);
				dup2(fd, 1);
				g_input->op = 1;
			}
		}
		else if (ft_strcmp(args[i], ">") == 0 && args[i + 1] && i != 0)
		{
			args[i + 1] = args_translate(args[i + 1]);
			if (fd > 0 && fd != 0 && fd != 1 && fd != 2)
				close(fd);
			if ((fd = open(args[i + 1], O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0)
			{
				input_error(args[i + 1]);
				if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
					close(fd);
				dup_std();
				return (fd);
			}
			else
			{
				ft_retab(args, i + 1);
				ft_retab(args, i);
				i--;
				if (g_input->std1 != 0)
					dup2(g_input->std1, 1);
				g_input->std1 = dup(1);
				dup2(fd, 1);
				g_input->op = 1;
			}
		}
		i++;
	}
//	printf("final fd: %d\n", fd);
	return (fd);
}

void	just_apply(char **line, char **args)
{
	if (args[0] && ft_strcmp(args[0], "echo") == 0)
		ft_echo(args);
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

void	ft_apply(char **line, char **arg)
{
	int		i;
	int		j;
	int		tube[2];
	char	**args;
	int		std;
	int		std1;

	if (!(args = (char **)malloc(sizeof(char *) * tab_size(arg) + 1)))
		exit(-1);
	i = 0;
	j = 0;
	tube[0] = 0;
	tube[1] = 0;
	std = dup(0);
	std1 = dup(1);
	while (arg[i])
	{
		args[j] = ft_strdup(arg[i]);
		if ((args[j][0] == 34 || args[j][0] == 39) && (ft_strchr(args[j], '|')))
		{
			free(args[j]);
			args[j] = ft_strdup("|");
		}
		else if (ft_strcmp(args[j], "|") == 0)
		{
			pipe(tube);
			dup2(tube[1], 1);
			free(args[j]);
			args[j] = NULL;
			just_apply(line, args);
			ft_tabdel(&args);
			j = -1;
			dup2(tube[0], 0);
			close(tube[1]);
		}
		if (g_data->error)
		{
			args[j] = NULL;
			dup2(std1, 1);
			dup2(std, 0);
			ioctl(0, TIOCSTI, "\n");
			ft_putstr("\033[F");
			ft_putstr("^C");
			return ;
		}
//		printf("args[%d]: %s\n", j, args[j]);
		j++;
		i++;
	}
	args[j] = NULL;
//	printf("after apply parsing (j: %d)\n", j);
//	ft_printtab(args);
	dup2(std1, 1);
	just_apply(line, args);
	dup2(std, 0);
	ft_tabdel(&args);
	free(args);
}

int		squote_invalid(char *line)
{
	int c;
	int i;

	c = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 39)
			c++;
		i++;
	}
	return (c % 2 ? 1 : 0);
}

void	get_quote(char **file, int mode)
{
	char	*new;
	char	*old;
	int		invalid;
	char	*line;

	line = *file;
	invalid = 0;
	new = NULL;
	if (mode == 34)
		invalid = quote_invalid(line);
	else if (mode == 39)
		invalid = squote_invalid(line);
	if (invalid)
	{
		old = ft_strdup(line);
		new = quote_mode(mode);
		if (new)
		{
			free(line);
			line = ft_strjoin(old, new);
			*file = line;
		}
		else
		{
			free(*file);
			*file = NULL;
		}
		free(old);
	}
}

void	quote_get2(char **file)
{
	int		i;
	int		sq;
	int		dq;
	char	*line = NULL;

	free(*file);
	*file = ft_strdup(*file);
	line = *file;
	sq = 0;
	dq = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 39 && !dq)
			sq = sq ? 0 : 1;
		if (line[i] == 34 && !sq)
			dq = dq ? 0 : 1;
		i++;
	}
	if (dq)
		get_quote(file, 34);
	else if (sq)
		get_quote(file, 39);
}

char		*var_translate(char *line, int i)
{
	char	*tmp;
	char	*env;
	int		found;
	int		len;
	char	*old;

	found = 0;
	env = NULL;
	old = ft_strdup(line + i  + 1);
//	ft_putstr(tmp);
	while (ft_isalnum(old[found]))
		found++;
	if (found)
		old[found] = '\0';
	tmp = ft_strjoin(old, "=");
	free(old);
	len = ft_strlen(tmp);
//	ft_putstr(tmp);
	env = get_var(g_data->cpy, tmp);
//	ft_putstr(env);
	while (len && found)
	{
		line = ft_delete(line, i, ft_strlen(line));
		len--;
	}
	if (env)
		line = insert_str(line, env, i, ft_strlen(line));
	free(tmp);
	return (line);
}

char		*point_translate(char *line, int i)
{
	int		found;
	char	*path;
	char	*tmp;

	found = 0;
	while (line[found + i] == '.')
		found++;
	if (ft_isalnum(line[found + i]))
		return (line);
	path = NULL;
	path = getcwd(path, 99);
	while (found)
	{
		if (found - 1 && (tmp = ft_strrchr(path, '/')))
			*tmp = '\0';
		line = ft_delete(line, i, ft_strlen(line));
		found--;
	}
	line = insert_str(line, path, i, ft_strlen(line));
	free(path);
	return (line);
}

char		*home_translate(char *line, int i)
{
	char	*env;
	int		found;

	found = 0;
	env = NULL;
	if (ft_isalnum(line[i + 1]))
		return (line);
	env = get_var(g_data->cpy, "HOME=");
	line = ft_delete(line, i, ft_strlen(line));
	if (env)
		line = insert_str(line, env, i, ft_strlen(line));
	return (line);
}

char		*remove_quote(char *line)
{
	char	*str;
	int		i;
	int		mode;
	char	*tmp;
	int		end;

//	ft_putstr("IN");
	mode = 0;
	i = 0;
	end = 0;
	str = ft_strdup("");
	while (line[i])
	{
//		ft_putnbr(i);
		if (line[i] == 34 && mode != 39)
		{
			if (mode)
			{
				mode = 0;
				end = 1;
			}
			else if (!mode)
				mode = 34;
		}
		else if (line[i] == 39 && mode != 34)
		{
			if (mode)
			{
				mode = 0;
				end = 1;
			}
			else if (!mode)
				mode = 39;
		}
		if (line[i] && line[i] != mode && !end)
		{
			tmp = ft_strdup(str);
			free(str);
			str = ft_charjoin(tmp, line[i]);
			free(tmp);
		}
		end = 0;
		i++;
	}
//	printf("str: %s\n", str);
	return (str);
}

char		*args_translate(char *line)
{
	int		i;
	char	*new;
	char	mode;

//	ft_putstr("IN");
	i = 0;
	mode = 0;
	if (ft_strcmp(line, "\"|\"") == 0)
		return (line);
	while (line[i])
	{
		if (line[i] == 34 && !mode)
			mode = mode ? 0 : 34;
		else if (line[i] == 39 && !mode)
			mode = mode ? 0 : 39;
		if (line[i] == '$' && mode != 39)
			line = var_translate(line, i);
		if (line[i] == '.' && mode != 39)
			line = point_translate(line, i);
		if (line[i] == '~' && mode != 39)
			line = home_translate(line, i);
		i++;
	}
//	ft_putstr("END_LOOP");
	new = remove_quote(line);
//	ft_putstr("end");
	free(line);
	return (new);
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
	tcsetattr(0, 0, g_data->bu);
	free(g_data->bu);
}

int			ft_minishell(char **line)
{
	char	**args;
	int		i;
	int		fd;

	i = 0;
	args = NULL;
	term_init();
//	printf("before quote_get\n");
//	ft_printtab(g_data->cpy);
	quote_get2(line);
//	printf("after quote_get\n");
//	ft_printtab(g_data->cpy);
	term_reset();
	args = get_a(*line, args);
//	ft_putstr("after get_a\n");
//	ft_putstr(*line);
//	ft_printtab(args);
	if ((fd = ft_redir(&args)) == -1)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		free(*line);
		ft_tabdel(&args);
		free(args);
		return (0);
	}
//	dprintf(2, "fd: %d\n", fd);
//	ft_printtab(args);
//	ft_putstr("after_redir\n");
//	ft_printtab(args);
	while (args[i])
	{
//		ft_putstr("IN");
		args[i] = args_translate(args[i]);
//		printf("args[i]: %s\n", args[i]);
		i++;
	}
//	ft_putstr("after_translate\n");
//	ft_putstr("after translate:\n");
//	ft_printtab(args);
//	ft_printtab(g_data->cpy);
	if (!args)
		ft_print_error(NULL, QUOTES, *line);
	else if (args[0] && ft_strcmp(args[0], "exit") == 0)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		ft_tabdel(&args);
		free(args);
		return (1);
	}
	else
		ft_apply(line, args);
//	ft_putstr("after apply:\n");
//	ft_putstr("end apply\n");
	if (args)
	{
		ft_tabdel(&args);
		free(args);
	}
	dup_std();
	if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
		close(fd);
	free(*line);
//	ft_putstr("after free\n");
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

static void		history_error()
{
	g_history->error = 1;
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": cant initialize history\n", 2);
}

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

static void	free_lines(void)
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

int		history_open(int fd)
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

static void	history_init(void)
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

static void	history_free(void)
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

static void	write_file(void)
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

int		ft_commands(char **line)
{
	char	*str = NULL;
	char	*found = NULL;
	char	*base = NULL;
	char	*tmp = NULL;

	base = ft_strdup(*line);
	str = ft_strdup(*line);
	while ((found = strchr_quote(str, ';')))
	{
		*(found + 1) = '\0';
		str[ft_strlen(str) - 1] = '\0';
		tmp = ft_strdup(base);
		free(base);
		base = ft_strdup(tmp + ft_strlen(str) + 1);
		if (ft_minishell(&str))
		{
			free(base);
			free(str);
			return (1);
		}
		free(tmp);
		if (g_data->error)
		{
			free(*line);
			free(base);
			return (0);
		}
		str = ft_strdup(base);
	}
	if (ft_minishell(&str))
	{
		free(base);
		free(str);
		return (1);
	}
	free(*line);
	free(base);
	return (0);
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

int			main(void)
{
	extern char	**environ;

	history_init();
	data_init();
	inputs_init();
	g_data->error = 0;
	environ_cpy(environ, &g_data->cpy);
	signal(SIGINT, inthandler);
	valid_term();
	set_lvl();
	while (1)
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
				break ;
		}
	}
	inputs_reset();
	data_free();
	history_free();
	return (0);
}

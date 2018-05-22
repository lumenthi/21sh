/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 10:11:36 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/22 12:48:19 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/21sh.h"

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
}

static int	heredoc_nb(char *cpy, char *found)
{
	char	*after;
	int		tube[2];

	*found = '\0';
	after = ft_strdup(found + 2);
	pipe(tube);
	term_init();
	write_mode(tube[1], after);
	term_reset();
	close(tube[1]);
	if (ft_atoi(cpy) == 0)
		g_input->std0 = dup(ft_atoi(cpy));
	else if (ft_atoi(cpy) == 1)
		g_input->std1 = dup(ft_atoi(cpy));
	else if (ft_atoi(cpy) == 2)
		g_input->std2 = dup(ft_atoi(cpy));
	dup2(tube[0], ft_atoi(cpy));
	g_input->op = 1;
	free(after);
	return (tube[0]);
}

static char	*get_after(char **found, char **cpy, int *sign, int *new_fd)
{
	char	*after;

	*sign = 1;
	if (**found == '<')
	{
		*sign = -1;
		if (*(*found + 1) == '<')
		{
			*sign = 2;
			*new_fd = heredoc_nb(*cpy, *found);
			free(*cpy);
			return (NULL);
		}
		after = ft_strdup(*found + 1);
	}
	else if (*(*found + 1) == '>')
	{
		after = ft_strdup(*found + 2);
		*sign = 2;
	}
	else
		after = ft_strdup(*found + 1);
	**found = '\0';
	return (after);
}

static void	not_filefd(int *new_fd, int sign, char *after)
{
	if (sign == 0)
		*new_fd = open(after, O_RDONLY);
	else if (sign == 1)
		*new_fd = open(after, O_RDWR|O_CREAT|O_TRUNC, 0666);
	else
		*new_fd = open(after, O_RDWR|O_CREAT|O_APPEND, 0666);
}

static int	open_fd(char *after, int sign)
{
	int		new_fd;

	new_fd = 0;
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
		not_filefd(&new_fd, sign, after);
	return (new_fd);
}

static void	numdup_badcpy(char **cpy, int *new_fd)
{
	int fd;

	fd = open(*cpy, O_RDONLY);
	if (fd > 0)
	{
		if (*new_fd == 0)
			g_input->std0 = dup(0);
		else if (*new_fd == 1)
			g_input->std1 = dup(1);
		else if (*new_fd == 2)
			g_input->std2 = dup(2);
		else
			return ;
		dup2(fd, *new_fd);
		close(fd);
		g_input->op = 1;
	}
	else
	{
		fd_error(*cpy);
		*new_fd = -2;
	}
}

static void	numdup_after(char **cpy, int *new_fd, int sign)
{
	if (ft_strcmp(*cpy, "") == 0)
	{
		g_input->std1 = dup(1);
		if (sign > 0)
			dup2(*new_fd, 1);
		else
			dup2(1, *new_fd);
		g_input->op = 1;
		return ;
	}
	else
		numdup_badcpy(cpy, new_fd);
}

static void	redir_numdup(char **cpy, int *new_fd, char **after, int sign)
{
	if (ft_strcmp(*cpy, "") != 0 && ft_isnum(*cpy))
	{
		if (ft_atoi(*cpy) == 0)
			g_input->std0 = dup(ft_atoi(*cpy));
		else if (ft_atoi(*cpy) == 1)
			g_input->std1 = dup(ft_atoi(*cpy));
		else if (ft_atoi(*cpy) == 2)
			g_input->std2 = dup(ft_atoi(*cpy));
		dup2(*new_fd, ft_atoi(*cpy));
		g_input->op = 1;
	}
	else
		numdup_after(cpy, new_fd, sign);
	free(*after);
	free(*cpy);
}

static int	invalid_fd(int new_fd, char **after, char **cpy, int sign)
{
	if (new_fd < 0)
	{
		if (sign != 0)
			fd_error(*after + 1);
		else
			fd_error(*after);
		free(*after);
		free(*cpy);
		return (1);
	}
	return (0);
}

static int	redir_init(char *line, int *new_fd, char **cpy)
{
	*new_fd = 0;
	*cpy = ft_strdup(line);
	if (ft_strcmp(line, ">") == 0 || ft_strcmp(line, "<") == 0 ||
	ft_strcmp(line, "<<") == 0 || ft_strcmp(line, ">>") == 0)
	{
		free(*cpy);
		return (0);
	}
	return (1);
}

static int	is_redir(char *line)
{
	char	*found;
	char	*after;
	char	*cpy;
	int		new_fd;
	int		sign;

	if (!redir_init(line, &new_fd, &cpy))
		return (-1);
	if ((found = ft_strchr(cpy, '<'))
	|| (found = ft_strchr(cpy, '>')))
	{
		if (!(after = get_after(&found, &cpy, &sign, &new_fd)) && new_fd != 0)
			return (new_fd);
		new_fd = open_fd(after, sign);
		if (invalid_fd(new_fd, &after, &cpy, sign))
			return (-2);
		redir_numdup(&cpy, &new_fd, &after, sign);
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
	while (args[i])
	{
		if (args[i][0] == 39 || args[i][0] == 34)
			;
		else if (ft_strcmp(args[i], ">>") != 0
		&& (ft_strcmp(args[i], "<<") != 0) && ((ret = is_redir(args[i])) >= 0)
			&& ft_strlen(args[i]) != 1)
		{
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

static void	get_afterredir(char **after, char **found, char **sign)
{
	if (*(*found + 1) == '<' || *(*found + 1) == '>')
	{
		*after = ft_strdup(*found + 2);
		*sign = ft_strdup(*found);
		*(*sign + 2) = '\0';
	}
	else
	{
		*after = ft_strdup(*found + 1);
		*sign = ft_strdup(*found);
		*(*sign + 1) = '\0';
	}
	**found = '\0';
}

static char	**tab_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	get_afterredir(&after, &found, &sign);
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

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	get_afterredir(&after, &found, &sign);
	free(args[i]);
	args[i] = ft_strdup(sign);
	args = tab_insert(args, i + 1, after);
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

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	get_afterredir(&after, &found, &sign);
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	free(after);
	free(sign);
	free(before);
	return (args);
}

static void	get_sign(char **found, char *line, char **sign)
{
	**found = '\0';
	if ((*found = strchr_quote(line, '>')))
	{
		if (*(--*found) == '>')
			*sign = ft_strdup(">>");
		else
			*sign = ft_strdup(">");
		++*found;
	}
	else
	{
		*found = strchr_quote(line, '<');
		if (*(--*found) == '<')
			*sign = ft_strdup("<<");
		else
			*sign = ft_strdup("<");
		++*found;
	}
}

static char **nb_lastredir(char **args, int i)
{
	char	*found;
	char	*line;
	char	*before;
	char	*sign;

	sign = NULL;
	line = ft_strdup(args[i]);
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	if (!found)
		return (args);
	get_sign(&found, line, &sign);
	free(args[i]);
	args[i] = ft_strjoin(before, sign);
	free(before);
	before = ft_strdup(args[i]);
	free(args[i]);
	args[i] = ft_strjoin(before, found + 1);
	if (sign)
		free(sign);
	free(line);
	free(before);
	return (args);
}

static void	get_mode(char *str, int i, int *mode)
{
	if (str[i] == 34 && *mode != 39)
	{
		if (*mode)
			*mode = 0;
		else if (!*mode)
			*mode = 34;
	}
	else if (str[i] == 39 && *mode != 34)
	{
		if (*mode)
			*mode = 0;
		else if (!*mode)
			*mode = 39;
	}
}

static int	opin_quote(char *str)
{
	int		i;
	int		mode;

	i = 0;
	mode = 0;
	while (str[i])
	{
		get_mode(str, i, &mode);
		if ((str[i] == '>' || str[i] == '<') && mode != 0)
			return (1);
		else if ((str[i] == '>' || str[i] == '<') && mode == 0)
			return (0);
		i++;
	}
	return (0);
}

static int	check_validity(char **found, char **tmp, int *other, int *j)
{
	if (ft_strlen(*found) < ft_strlen(*tmp))
		*found = *tmp;
	if (*(*found + 0) == '<')
		*other = '>';
	else if (*(*found + 0) == '>')
		*other = '<';
	while (*(*found + *j) == *(*found + 0))
		(*j)++;
	if (*j > 2 || *(*found + *j) == '|'
	|| *(*found + *j) == ';' || *(*found + *j) == *other)
		return (0);
	if (!valid_redir(*found + *j))
		return (0);
	return (1);
}

int		valid_redir(char *str)
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
		if (!check_validity(&found, &tmp, &other, &j))
			return (0);
	}
	return (1);
}

static char	*strrchr_quote(char *line, int elem)
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
			found = line + i;
		i++;
	}
	return (found);
}

static void	get_found(char **found, char **after, char *cpy, char **str)
{
	if (!(*found = strrchr_quote(cpy, '<')))
		*found = strrchr_quote(cpy, '>');
	if (*(*found - 1) == '>' || *(*found - 1) == '<')
		*found = *found - 1;
	*after = ft_strdup(*found);
	if (!(*found = ft_strchr(cpy, '<')))
		*found = ft_strchr(cpy, '>');
	**found = '\0';
	free(*str);
	*str = ft_strjoin(cpy, *after);
}

static int	arg_last_redir(char **str, char **cpy2)
{
	char	*found;
	char	*after;
	char	*cpy;

	if (!(valid_redir(*str)))
	{
		free(*cpy2);
		parse_error();
		return (-1);
	}
	cpy = ft_strdup(*str);
	get_found(&found, &after, cpy, str);
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

static void		nb_retabs(char ***args, int i, char *after)
{
	char	*final;

	if (ft_strcmp(after, "") == 0 || ft_strcmp(after, ">") == 0
	|| ft_strcmp(after, "<") == 0)
	{
		*args = nb_lastredir(*args, i);
		final = ft_strjoin(*(*args + i), *(*args + i + 1));
		free(*(*args + i));
		*(*args + i) = ft_strdup(final);
		ft_retab(*args, i + 1);
		free(final);
	}
	*args = nb_lastredir(*args, i);
	final = ft_strdup(*(*args + i));
	free(*(*args + i));
	*(*args + i) = remove_quote(final);
	free(final);
}

static int		retab_argsdir(char ***args, char **cpy, char **after, int i)
{
	if (!*(*args + i + 1) && (ft_strcmp(*after, "") == 0 ||
	ft_strcmp(*after, ">") == 0 || ft_strcmp(*after, "<") == 0))
	{
		parse_error();
		free(*cpy);
		free(*after);
		return (0);
	}
	if (ft_isnum(*cpy) && ft_strcmp(*cpy, "") != 0)
		nb_retabs(args, i, *after);
	else if (ft_strcmp(*cpy, "") == 0)
		*args = before_resize(*args, i);
	else if (ft_strcmp(*after, "") == 0 ||
		ft_strcmp(*after, ">") == 0 || ft_strcmp(*after, "<") == 0)
		*args = after_resize(*args, i);
	else
		*args = tab_resize(*args, i);
	free(*after);
	return (1);
}

static int		retab_condition(char **found, char *cpy, char *args)
{
	if (!opin_quote(cpy) && ((*found = strchr_quote(cpy, '>')) ||
		(*found = strchr_quote(cpy, '<'))) &&
		ft_strcmp(cpy, ">>") != 0 && (ft_strcmp(cpy, "<<") != 0) &&
		ft_strlen(args) != 1)
		return (1);
	return (0);
}

static void		retab_dirsinit(int *i, int *ret)
{
	*i = 0;
	*ret = 0;
}

static void		loop_forward(int *i, char **cpy)
{
	free(*cpy);
	(*i)++;
}

static void		retab_norm(char **after, char **found)
{
	*after = ft_strdup(*found + 1);
	**found = '\0';
}

char			**retab_dirs(char **args)
{
	int		i;
	char	*after;
	char	*found;
	char	*cpy;
	int		ret;

	retab_dirsinit(&i, &ret);
	while (args[i])
	{
		cpy = ft_strdup(args[i]);
		if (retab_condition(&found, cpy, args[i]))
		{
			if ((ret = arg_last_redir(&args[i], &cpy)) == -1)
				return (NULL);
			else if (ret != 0)
			{
				retab_norm(&after, &found);
				if (!retab_argsdir(&args, &cpy, &after, i))
					return (NULL);
			}
		}
		loop_forward(&i, &cpy);
	}
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

static int	get_pipefound(char **found, char **cpy, int i, char **args)
{
	if (*(*found + 1) == '|' || *(*found + 1) == '<' ||
		*(*found + 1) == '>')
	{
		parse_error();
		free(*cpy);
		return (0);
	}
	if (ft_strlen(args[i]) != ft_strlen(*found))
	{
		if (*(*found - 1) == '<' || *(*found - 1) == '>')
		{
			parse_error();
			free(*cpy);
			return (0);
		}
	}
	return (1);
}

static void	change_args(char ***args, int i, char **after, char *cpy)
{
	if (ft_strcmp(cpy, "") == 0)
		*args = before_pipes(*args, i);
	else if (ft_strcmp(*after, "") == 0)
		*args = after_pipes(*args, i);
	else
		*args = resize_pipes(*args, i);
	free(*after);
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
			if (!get_pipefound(&found, &cpy, i, args))
				return (NULL);
			after = ft_strdup(found + 1);
			*found = '\0';
			change_args(&args, i, &after, cpy);
		}
		free(cpy);
		i++;
	}
	return (args);
}

static char		**redit_init(int tube[], char ***arg, int *i, int *fd)
{
	char **args;

	args = NULL;
	tube[0] = 0;
	tube[1] = 0;
	g_input->op = 0;
	g_input->std0 = 0;
	g_input->std1 = 0;
	g_input->std2 = 0;
	*fd = 0;
	*i = 0;
	if (!(args = retab_pipes(*arg)))
		return (NULL);
	if (!(args = retab_dirs(args)))
		return (NULL);
	else
		*arg = args;
	*fd = first_redir(args);
	return (args);
}

static int		apply_heredoc(char ***args, int tube[], int i)
{
	if (!*(*args + (i + 1)))
	{
		parse_error();
		return (0);
	}
	if (tube[0] != 0)
	{
		ft_retab(*args, i + 1);
		ft_retab(*args, i);
	}
	else
	{
		if (g_input->std0 != 0)
			dup2(g_input->std0, 0);
		pipe(tube);
		term_init();
		write_mode(tube[1], *(*args + (i + 1)));
		term_reset();
		ft_retab(*args, i + 1);
		ft_retab(*args, i);
		close(tube[1]);
	}
	return (1);
}

static int		get_heredoc(char ***args, int tube[])
{
	int i;

	i = 0;
	while (*(*args + i))
	{
		if (ft_strcmp(*(*args + i), "<<") == 0)
		{
			if (!apply_heredoc(args, tube, i))
				return (0);
		}
		if (g_data->error == 1)
		{
			print_prompt(g_data->cpy);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	redir_sleftend(int *fd, int tube[])
{
	if (g_input->std0 != 0)
		dup2(g_input->std0, 0);
	g_input->std0 = dup(0);
	dup2(*fd, 0);
	g_input->op = 1;
	if (tube[0] != 0)
	{
		close(tube[0]);
		tube[0] = 0;
	}

}

static int	redir_sleft(char ***args, int *i, int *fd, int tube[])
{
	if (*(*args + (*i + 1)))
		*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if (tube[0] != 0)
		*fd = tube[0];
	else
	{
		if ((*fd = open(*(*args + (*i + 1)), O_RDWR)) < 0)
		{
			input_error(*(*args + (*i + 1)));
			if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
				close(*fd);
			dup_std();
			return (0);
		}
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
	}
	redir_sleftend(fd, tube);
	return (1);
}

static int	redir_dright(char ***args, int *i, int *fd)
{
	*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if ((*fd = open(*(*args + (*i + 1)), O_RDWR|O_CREAT|O_APPEND, 0666)) < 0)
	{
		input_error(*(*args + (*i + 1)));
		if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
			close(*fd);
		dup_std();
		return (0);
	}
	else
	{
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
		if (g_input->std1 != 0)
			dup2(g_input->std1, 1);
		g_input->std1 = dup(1);
		dup2(*fd, 1);
		g_input->op = 1;
	}
	return (1);
}

static int	redir_sright(char ***args, int *i, int *fd)
{
	*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if ((*fd = open(*(*args + (*i + 1)), O_RDWR|O_CREAT|O_TRUNC, 0666)) < 0)
	{
		input_error(*(*args + (*i + 1)));
		if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
			close(*fd);
		dup_std();
		return (0);
	}
	else
	{
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
		if (g_input->std1 != 0)
			dup2(g_input->std1, 1);
		g_input->std1 = dup(1);
		dup2(*fd, 1);
		g_input->op = 1;
	}
	return (1);
}

int		ft_redir(char ***arg)
{
	int		i;
	int		fd;
	int		tube[2];
	char	**args;

	if ((args = redit_init(tube, arg, &i, &fd)) == NULL)
		return (-1);
	if (!get_heredoc(&args, tube))
		return (-1);
	while (args[i])
	{
		if (((ft_strcmp(args[i], "<") == 0 && args[i + 1]) || tube[0] != 0) &&
		!redir_sleft(&args, &i, &fd, tube))
			return (fd);
		else if ((ft_strcmp(args[i], ">>") == 0 && args[i + 1] && i != 0) &&
		!redir_dright(&args, &i, &fd))
			return (fd);
		else if ((ft_strcmp(args[i], ">") == 0 && args[i + 1] && i != 0) &&
		!redir_sright(&args, &i, &fd))
			return (fd);
		i++;
	}
	return (fd);
}

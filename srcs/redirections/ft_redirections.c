/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 10:11:36 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 10:26:05 by lumenthi         ###   ########.fr       */
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

static int	is_redir(char *line)
{
	char	*found;
	char	*after;
	char	*cpy;
	int		new_fd;
	int		sign;

	new_fd = 0;
	cpy = ft_strdup(line);
	if (ft_strcmp(line, ">") == 0 || ft_strcmp(line, "<") == 0 ||
	ft_strcmp(line, "<<") == 0 || ft_strcmp(line, ">>") == 0)
	{
		free(cpy);
		return (-1);
	}
	if ((found = ft_strchr(cpy, '<'))
	|| (found = ft_strchr(cpy, '>')))
	{
		if (*found == '<')
		{
			if (*(found + 1) == '<')
			{
				new_fd = heredoc_nb(cpy, found);
				free(cpy);
				return (new_fd);
			}
			else
				after = ft_strdup(found + 1);
			sign = 2;
		}
		else if (*(found + 1) == '>')
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
	while (args[i])
	{
		if (args[i][0] == 39 || args[i][0] == 34)
			;
		else if (ft_strcmp(args[i], ">>") != 0 && (ft_strcmp(args[i], "<<") != 0)
			&& ((ret = is_redir(args[i])) >= 0) && ft_strlen(args[i]) != 1)
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
	char	*sign;

	sign = NULL;
	line = ft_strdup(args[i]);
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	if (!found)
		return (args);
	*found = '\0';
	if ((found = ft_strrchr(line, '>')))
	{
		if (*(--found) == '>')
			sign = ft_strdup(">>");
		else
			sign = ft_strdup(">");
		++found;
	}
	else
	{
		found = ft_strrchr(line, '<');
		if (*(--found) == '<')
			sign = ft_strdup("<<");
		else
			sign = ft_strdup("<");
		++found;
	}
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
	if (*(found - 1) == '>' || *(found - 1) == '<')
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
char	**retab_dirs(char **args)
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
				if (ft_isnum(cpy) && ft_strcmp(cpy, "") != 0)
					args = nb_lastredir(args, i);
				else if (ft_strcmp(cpy, "") == 0)
					args = before_resize(args, i);
				else if (ft_strcmp(after, "") == 0 ||
					ft_strcmp(after, ">") == 0 || ft_strcmp(after, "<") == 0)
					args = after_resize(args, i);
				else
					args = tab_resize(args, i);
				free(after);
			}
		}
		free(cpy);
		i++;
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

int		ft_redir(char ***arg)
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
	if (!(args = retab_dirs(args)))
		return (-1);
	else
		*arg = args;
	fd = first_redir(args);
	while (args[i])
	{
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
				pipe(tube);
				term_init();
				write_mode(tube[1], args[i + 1]);
				term_reset();
				ft_retab(args, i + 1);
				ft_retab(args, i);
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
				args[i + 1] = args_translate(args[i + 1], args);
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
			args[i + 1] = args_translate(args[i + 1], args);
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
			args[i + 1] = args_translate(args[i + 1], args);
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
	return (fd);
}

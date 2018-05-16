/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 10:58:08 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/15 14:33:52 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static int		squote_invalid(char *line)
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

static void	make_newstring(char **line, char **old, char **new, char **file)
{
	if (*new)
	{
		free(*line);
		*line = ft_strjoin(*old, *new);
		*file = *line;
	}
	else
	{
		free(*file);
		*file = NULL;
	}
}

static void	get_quote(char **file, int mode)
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
		old = ft_strjoin(line, "\n");
		new = quote_mode(mode);
		make_newstring(&line, &old, &new, file);
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

static void		var_delete(char **line, int *i, int *len)
{
	*line = ft_delete(*line, *i, ft_strlen(*line));
	(*len)--;
}

static char		*var_translate(char *line, int i)
{
	char	*tmp;
	char	*env;
	int		found;
	int		len;
	char	*old;

	found = 0;
	env = NULL;
	old = ft_strdup(line + i  + 1);
	while (ft_isalnum(old[found]))
		found++;
	if (found)
		old[found] = '\0';
	tmp = ft_strjoin(old, "=");
	free(old);
	len = ft_strlen(tmp);
	env = get_var(g_data->cpy, tmp);
	while (len && found)
		var_delete(&line, &i, &len);
	if (env)
		line = insert_str(line, env, i, ft_strlen(line));
	free(tmp);
	return (line);
}

static char		*point_translate(char *line, int i)
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

static char		*home_translate(char *line, int i)
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

static void		get_currentq(char *line, int *mode, int *end, int i)
{
	if (line[i] == 34 && *mode != 39)
	{
		if (*mode)
		{
			*mode = 0;
			*end = 1;
		}
		else if (!*mode)
			*mode = 34;
	}
	else if (line[i] == 39 && *mode != 34)
	{
		if (*mode)
		{
			*mode = 0;
			*end = 1;
		}
		else if (!*mode)
			*mode = 39;
	}
}

char		*remove_quote(char *line)
{
	char	*str;
	int		i;
	int		mode;
	char	*tmp;
	int		end;

	mode = 0;
	i = 0;
	end = 0;
	str = ft_strdup("");
	while (line[i])
	{
		get_currentq(line, &mode, &end, i);
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
	return (str);
}

static void	variables_init(int *i, char *mode)
{
	*i = 0;
	*mode = 0;
}

char		*args_translate(char *line, char **args)
{
	int		i;
	char	*new;
	char	mode;

	variables_init(&i, &mode);
	if (ft_strcmp(line, "\"|\"") == 0)
		return (line);
	while (line[i])
	{
		if (line[i] == 34 && mode != 39)
			mode = mode ? 0 : 34;
		else if (line[i] == 39 && mode != 34)
			mode = mode ? 0 : 39;
		if (line[i] == '$' && mode != 39 && ft_strcmp(args[0], "env") != 0)
			line = var_translate(line, i);
		if (line[i] == '.' && mode != 39 && ft_strcmp(args[0], "echo") != 0)
			line = point_translate(line, i);
		if (line[i] == '~' && mode != 39)
			line = home_translate(line, i);
		i++;
	}
	new = remove_quote(line);
	free(line);
	return (new);
}

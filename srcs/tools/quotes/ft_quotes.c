/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 10:58:08 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:29:21 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

static int	squote_invalid(char *line)
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

void		quote_get2(char **file)
{
	int		i;
	int		sq;
	int		dq;
	char	*line;

	line = NULL;
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

char		*args_translate(char *line, char **args)
{
	int		i;
	char	*new;
	char	mode;

	variables_init(&i, &mode);
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
	translate_end(&new, line, args);
	free(line);
	return (new);
}

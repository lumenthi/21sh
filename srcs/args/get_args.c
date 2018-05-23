/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 10:32:01 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 14:56:22 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	make_word(char ***args, int *i, char **str, int j)
{
	if (*str)
	{
		*(*str + j) = '\0';
		*args = (char **)ft_realloc(*args, sizeof(char *) * (*i + 1));
		*(*args + *i) = ft_strdup(*str);
		free(*str);
		*str = NULL;
		(*i)++;
	}
}

static void	get_quotes(int *c, char *line, char **str, int *j)
{
	int		lim;

	lim = (int)ft_strlen(line);
	if (!(*str))
	{
		if (!(*str = malloc(lim)))
			exit(-1);
	}
	*(*str + *j) = line[*c];
	(*c)++;
	(*j)++;
	while (*c <= lim)
	{
		*(*str + *j) = line[*c];
		(*j)++;
		if (line[*c] == 34)
		{
			if (*c == lim - 1)
				break ;
			else if (ft_isspace(line[(*c) + 1]))
				break ;
		}
		(*c)++;
	}
}

static void	get_squotes(int *c, char *line, char **str, int *j)
{
	int		lim;

	lim = (int)ft_strlen(line);
	if (!(*str))
	{
		if (!(*str = malloc(lim)))
			exit(-1);
	}
	*(*str + *j) = line[*c];
	(*c)++;
	(*j)++;
	while (*c <= lim)
	{
		*(*str + *j) = line[*c];
		(*j)++;
		if (line[*c] == 39)
		{
			if (*c == lim - 1)
				break ;
			else if (ft_isspace(line[(*c) + 1]))
				break ;
		}
		(*c)++;
	}
}

static void	get_words(char *line, char ***args, char **str, int *i)
{
	int	lim;
	int	c;
	int	j;

	lim = ft_strlen(line);
	c = 0;
	j = 0;
	while (c < lim)
	{
		if (line[c] == 34 && dquotes_valid(line, c))
			get_quotes(&c, line, str, &j);
		else if (line[c] == 39)
		{
			if (c == 0)
				get_squotes(&c, line, str, &j);
			else if (ft_isspace(line[c - 1]))
				get_squotes(&c, line, str, &j);
		}
		else
			get_normal(&c, line, str, &j);
		make_word(args, i, str, j);
		j = 0;
		c++;
	}
}

char		**get_a(char *line, char **args)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	args = NULL;
	get_words(line, &args, &str, &i);
	args = (char **)ft_realloc(args, sizeof(char *) * (i + 1));
	args[i] = NULL;
	free(str);
	return (args);
}

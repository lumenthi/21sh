/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/08 10:32:01 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/13 11:49:11 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static void	get_normal(int *c, char *line, char **str, int *j)
{
	int lim;

	lim = (int)ft_strlen(line);
	while (*c < lim && !ft_is_space(line[*c]))
	{
		if (!(*str))
			*str = malloc(lim);
		*(*str + *j) = line[*c];
		(*j)++;
		(*c)++;
	}
}

static void	get_quotes(int *c, char *line, char **str, int *j)
{
	int		lim;

	lim = (int)ft_strlen(line);
	(*c)++;
	if (line[*c] == '"')
		*str = ft_strdup("");
	else
	{
		while (*c < lim && line[*c] != '"')
		{
			if (!(*str))
				*str = malloc(lim);
			*(*str + *j) = line[*c];
			(*j)++;
			(*c)++;
		}
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
		if (line[c] == '"')
			get_quotes(&c, line, str, &j);
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

	if (line && quote_invalid(line))
		return (NULL);
	i = 0;
	str = NULL;
	args = NULL;
	get_words(line, &args, &str, &i);
	args = (char **)ft_realloc(args, sizeof(char *) * (i + 1));
	args[i] = NULL;
	free(str);
	return (args);
}

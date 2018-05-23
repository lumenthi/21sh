/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:20:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:30:08 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

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

static void	get_currentq(char *line, int *mode, int *end, int i)
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

void		variables_init(int *i, char *mode)
{
	*i = 0;
	*mode = 0;
}

void		translate_end(char **ne, char *line, char **args)
{
	if (ft_strcmp(args[0], "env") != 0)
		*ne = remove_quote(line);
	else
		*ne = ft_strdup(line);
}

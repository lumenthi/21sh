/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:31:07 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 22:32:57 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	data_free(void)
{
	ft_tabdel(&g_data->cpy);
	free(g_data->cpy);
	free(g_data->cursor);
	free(g_data->line);
	free(g_data);
}

void	data_init(void)
{
	if (!(g_data = (t_data *)malloc(sizeof(t_data))))
		malloc_error();
	if (!(g_data->cursor = (t_cursor *)malloc(sizeof(t_cursor))))
		malloc_error();
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

int		end_commands(char **str, char **base, char **line)
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

void		loop_init(char **found, char **str, char **base, char **tmp)
{
	*(*found + 1) = '\0';
	*(*str + ft_strlen(*str) - 1) = '\0';
	*tmp = ft_strdup(*base);
	free(*base);
	*base = ft_strdup(*tmp + ft_strlen(*str) + 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quotes3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:27:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 15:35:27 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

static void		var_delete(char **line, int *i, int *len)
{
	*line = ft_delete(*line, *i, ft_strlen(*line));
	(*len)--;
}

char			*var_translate(char *line, int i)
{
	char	*tmp;
	char	*env;
	int		found;
	int		len;
	char	*old;

	found = 0;
	env = NULL;
	old = ft_strdup(line + i + 1);
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

char			*point_translate(char *line, int i)
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
	if (!path)
		path = ft_strdup("");
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

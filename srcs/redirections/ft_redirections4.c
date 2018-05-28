/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:28:38 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 14:03:54 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		redir_init(char *line, int *new_fd, char **cpy)
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

int		is_redir(char *line)
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

int		first_redir(char **args)
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

char	**tab_insert(char **args, int i, char *ne)
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
	args[c] = ft_strdup(ne);
	args[size] = NULL;
	return (args);
}

void	get_afterredir(char **after, char **found, char **sign)
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

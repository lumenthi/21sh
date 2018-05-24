/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:43:12 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 21:45:00 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	get_mode(char *str, int i, int *mode)
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

int		opin_quote(char *str)
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

int		check_validity(char **found, char **tmp, int *other, int *j)
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

char	*strrchr_quote(char *line, int elem)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 14:52:43 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 16:31:24 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		dquotes_valid(char *line, int c)
{
	if (c == 0)
		return (1);
	else if (ft_isspace(line[c - 1]))
		return (1);
	return (0);
}

void	get_normal(int *c, char *line, char **str, int *j)
{
	int lim;

	lim = (int)ft_strlen(line);
	while (*c < lim && !ft_isspace(line[*c]))
	{
		if (!(*str))
		{
			if (!(*str = malloc(lim)))
				malloc_error();
		}
		*(*str + *j) = line[*c];
		(*j)++;
		(*c)++;
	}
}

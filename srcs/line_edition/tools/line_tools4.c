/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_tools4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:10:51 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:11:26 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

char	*ft_insert(char *line, char buf, int pos, int i)
{
	char	*after;

	if (i == 0)
	{
		free(line);
		line = ft_strdup("");
	}
	else
		line[i] = '\0';
	if (!(after = malloc(i + 2)))
		exit(-1);
	ft_strncpy(after, line, pos);
	after[pos] = buf;
	after[pos + 1] = '\0';
	ft_strcat(after, line + pos);
	after[i + 1] = '\0';
	if (line)
		free(line);
	return (after);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_history.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 14:03:18 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/09 14:05:15 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/21sh.h"

void	history_search(int *i, char a)
{
	int		pos;

	if (a == 'u' && g_history->position <= g_history->nb_lines)
	{
		if (g_history->position == g_history->nb_lines)
			g_history->position--;
		pos = g_history->nb_lines - g_history->position - 1;
		free(g_data->line);
		g_data->line = ft_strdup(g_history->line[pos]);
		g_history->position++;
	}
	else if (a == 'd' && g_history->position >= 1)
	{
		if (g_history->position == 1)
		{
			g_history->position--;
			free(g_data->line);
			g_data->line = NULL;
		}
		else
		{
			g_history->position--;
			pos = g_history->nb_lines - g_history->position;
			free(g_data->line);
			g_data->line = ft_strdup(g_history->line[pos]);
		}
	}
	ft_rewrite(i);
}

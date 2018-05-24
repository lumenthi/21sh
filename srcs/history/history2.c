/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 14:59:16 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 16:31:55 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int				history_open(int fd)
{
	if (g_history->nb_lines < HISTORY_LIMIT)
		fd = open(HISTORY_PATH, O_RDWR | O_CREAT | O_APPEND, 0666);
	else
	{
		fd = open(HISTORY_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666);
		ft_putstr(BLUE);
		ft_putstr("History limit reached, cleaning history...\n");
		ft_putstr(BLANK);
		g_history->nb_lines = 0;
	}
	return (fd);
}

static void		first_init(int *i, int *fd)
{
	*i = 0;
	*fd = 0;
	if (!(g_history = malloc(sizeof(t_history))))
		malloc_error();
	if (!(g_history->line = malloc(sizeof(char *) * HISTORY_LIMIT + 1)))
		malloc_error();
	g_history->nb_lines = 0;
	g_history->error = 0;
	g_history->special = 0;
	*fd = history_open(*fd);
}

void			history_init(void)
{
	int i;
	int	fd;

	first_init(&i, &fd);
	if (HISTORY_LIMIT < 0)
	{
		history_error();
		g_history->position = 0;
		return ;
	}
	if (fd > 0)
	{
		while (i <= HISTORY_LIMIT)
		{
			g_history->line[i] = NULL;
			i++;
		}
		get_lines();
		close(fd);
	}
	else if (fd < 0)
		history_error();
	g_history->position = 0;
}

void			history_free(void)
{
	free_lines();
	free(g_history->line);
	free(g_history);
}

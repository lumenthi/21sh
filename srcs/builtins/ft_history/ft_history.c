/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 16:00:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/08 16:03:23 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

void	ft_history(char **args)
{
	int	i;
	int	fd;

	i = 0;
	if (tab_size(args) == 1)
	{
		ft_putendl("history:\n");
		if (g_history->nb_lines <= 0)
			return ;
		while (g_history->line[i + 1])
		{
			ft_putnbr(i + 1);
			ft_putstr(" - [");
			ft_putstr(g_history->line[i]);
			ft_putendl("]");
			i++;
		}
	}
	else if (ft_strcmp(args[1], "clean") == 0 ||
		ft_strcmp(args[1], "clear") == 0 || ft_strcmp(args[1], "reset") == 0)
	{
		fd = open(HISTORY_PATH, O_RDWR|O_CREAT|O_TRUNC, 0666);
		if (fd < 0)
		{
			g_history->error = 1;
			history_error();
			return ;
		}
		else
			g_history->error = 0;
		free_lines();
		ft_putstr(GREEN);
		ft_putstr("Cleaning history...");
		ft_putstr(BLANK);
		ft_putchar('\n');
		g_history->nb_lines = 0;
		close(fd);
	}
}

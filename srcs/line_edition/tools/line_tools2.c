/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_tools2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:01:40 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:04:20 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	ft_end(int i)
{
	while (ft_move('r', i))
		;
}

void	line_up(int i)
{
	int j;

	j = 0;
	while (j != g_data->w_col)
	{
		if (!ft_move('l', i))
			break ;
		j++;
	}
}

void	line_down(int i)
{
	int j;

	j = 0;
	while (j != g_data->w_col)
	{
		if (!ft_move('r', i))
			break ;
		j++;
	}
}

void	word_left(int i)
{
	while (ft_move('l', i))
	{
		if (ft_isalnum(g_data->line[g_data->pos]) &&
			ft_isspace(g_data->line[g_data->pos + 1]))
			break ;
	}
}

void	word_right(int i)
{
	while (ft_move('r', i))
	{
		if (ft_isalnum(g_data->line[g_data->pos]) &&
			ft_isspace(g_data->line[g_data->pos - 1]))
			break ;
	}
}

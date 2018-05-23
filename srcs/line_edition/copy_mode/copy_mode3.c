/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_mode3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 15:33:45 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 15:36:58 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	copy_cut(int *i, char **cpy, int *select)
{
	mode_icon('P', *i);
	copy_sub(ft_strlen(*cpy), i);
	copy_write(g_data->pos, i);
	*select = 1;
}

int		copy_paste(int *i, char **cpy, int *select)
{
	g_data->line = insert_str(g_data->line, *cpy, g_data->pos, *i);
	*select = 0;
	ft_clear(*i);
	*i = (int)ft_strlen(g_data->line);
	ft_rewrite(i);
	return (1);
}

int		copy_quit(int *i, int select)
{
	if (select)
		ft_put("ue");
	copy_write(g_data->pos, i);
	return (1);
}

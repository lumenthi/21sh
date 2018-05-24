/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:15:46 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 21:20:10 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		open_fd(char *after, int sign)
{
	int		new_fd;

	new_fd = 0;
	if (ft_strchr(after, '&'))
	{
		if (ft_strcmp(after, "&-") == 0)
			new_fd = open("/dev/null", O_RDONLY);
		else if (ft_strcmp(after, "&0") == 0 || ft_strcmp(after, "&1") == 0
			|| ft_strcmp(after, "&2") == 0)
			new_fd = ft_atoi(after + 1);
		else
		{
			if (sign == 1)
				new_fd = open(after + 1, O_RDWR);
			else
				new_fd = open(after + 1, O_RDWR|O_APPEND);
		}
	}
	else
		not_filefd(&new_fd, sign, after);
	return (new_fd);
}

void	numdup_badcpy(char **cpy, int *new_fd)
{
	int fd;

	fd = open(*cpy, O_RDONLY);
	if (fd > 0)
	{
		if (*new_fd == 0)
			g_input->std0 = dup(0);
		else if (*new_fd == 1)
			g_input->std1 = dup(1);
		else if (*new_fd == 2)
			g_input->std2 = dup(2);
		else
			return ;
		dup2(fd, *new_fd) == -1 ? dup_error() : 1;
		close(fd);
		g_input->op = 1;
	}
	else
	{
		fd_error(*cpy);
		*new_fd = -2;
	}
}

void	numdup_after(char **cpy, int *new_fd, int sign)
{
	if (ft_strcmp(*cpy, "") == 0)
	{
		g_input->std1 = dup(1);
		if (sign > 0)
			dup2(*new_fd, 1) == -1 ? dup_error() : 1;
		else
			dup2(1, *new_fd) == -1 ? dup_error() : 1;
		g_input->op = 1;
		return ;
	}
	else
		numdup_badcpy(cpy, new_fd);
}

void	redir_numdup(char **cpy, int *new_fd, char **after, int sign)
{
	if (ft_strcmp(*cpy, "") != 0 && ft_isnum(*cpy))
	{
		if (ft_atoi(*cpy) == 0)
			g_input->std0 = dup(ft_atoi(*cpy));
		else if (ft_atoi(*cpy) == 1)
			g_input->std1 = dup(ft_atoi(*cpy));
		else if (ft_atoi(*cpy) == 2)
			g_input->std2 = dup(ft_atoi(*cpy));
		dup2(*new_fd, ft_atoi(*cpy)) == 1 ? dup_error() : 1;
		g_input->op = 1;
	}
	else
		numdup_after(cpy, new_fd, sign);
	free(*after);
	free(*cpy);
}

int		invalid_fd(int new_fd, char **after, char **cpy, int sign)
{
	if (new_fd < 0)
	{
		if (sign != 0)
			fd_error(*after + 1);
		else
			fd_error(*after);
		free(*after);
		free(*cpy);
		return (1);
	}
	return (0);
}

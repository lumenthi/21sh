/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 10:11:36 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 11:16:05 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	redir_sleft(char ***args, int *i, int *fd, int tube[])
{
	if (*(*args + (*i + 1)))
		*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if (tube[0] != 0)
		*fd = tube[0];
	else
	{
		if ((*fd = open(*(*args + (*i + 1)), O_RDWR)) < 0)
		{
			input_error(*(*args + (*i + 1)));
			if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
				close(*fd);
			dup_std();
			return (0);
		}
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
	}
	redir_sleftend(fd, tube);
	return (1);
}

static int	redir_dright(char ***args, int *i, int *fd)
{
	*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if ((*fd = open(*(*args + (*i + 1)),
	O_RDWR | O_CREAT | O_APPEND, 0666)) < 0)
	{
		input_error(*(*args + (*i + 1)));
		if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
			close(*fd);
		dup_std();
		return (0);
	}
	else
	{
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
		if (g_input->std1 != 0)
			dup2(g_input->std1, 1);
		g_input->std1 = dup(1);
		dup2(*fd, 1);
		g_input->op = 1;
	}
	return (1);
}

static int	redir_sright(char ***args, int *i, int *fd)
{
	*(*args + (*i + 1)) = args_translate(*(*args + (*i + 1)), *args);
	if (*fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
	if ((*fd = open(*(*args + (*i + 1)), O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		input_error(*(*args + (*i + 1)));
		if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
			close(*fd);
		dup_std();
		return (0);
	}
	else
	{
		ft_retab(*args, *i + 1);
		ft_retab(*args, *i);
		(*i)--;
		if (g_input->std1 != 0)
			dup2(g_input->std1, 1);
		g_input->std1 = dup(1);
		dup2(*fd, 1);
		g_input->op = 1;
	}
	return (1);
}

int			ft_redir(char ***arg)
{
	int		i;
	int		fd;
	int		tube[2];
	char	**args;

	if ((args = redit_init(tube, arg, &i, &fd)) == NULL)
		return (-1);
	if (!get_heredoc(&args, tube))
		return (-1);
	while (args[i])
	{
		if (((ft_strcmp(args[i], "<") == 0 && args[i + 1]) || tube[0] != 0) &&
		!redir_sleft(&args, &i, &fd, tube))
			return (fd);
		else if ((ft_strcmp(args[i], ">>") == 0 && args[i + 1] && i != 0) &&
		!redir_dright(&args, &i, &fd))
			return (fd);
		else if ((ft_strcmp(args[i], ">") == 0 && args[i + 1] && i != 0) &&
		!redir_sright(&args, &i, &fd))
			return (fd);
		i++;
	}
	return (fd);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:09:44 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 22:12:38 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char		**redit_init(int tube[], char ***arg, int *i, int *fd)
{
	char **args;

	args = NULL;
	tube[0] = 0;
	tube[1] = 0;
	g_input->op = 0;
	g_input->std0 = 0;
	g_input->std1 = 0;
	g_input->std2 = 0;
	*fd = 0;
	*i = 0;
	if (!(args = retab_pipes(*arg)))
		return (NULL);
	if (!(args = retab_dirs(args)))
		return (NULL);
	else
		*arg = args;
	*fd = first_redir(args);
	return (args);
}

void		do_heredoc(char ***args, int tube[], int i)
{
	int old_dup;

	if (g_input->std0 != 0)
		dup2(g_input->std0, 0) == -1 ? dup_error() : 1;
	if (g_pipe->in == 1)
	{
		old_dup = dup(1);
		dup2(g_input->std1, 1) == -1 ? dup_error() : 1;
	}
	pipe(tube) == -1 ? pipe_error() : 1;
	term_init();
	write_mode(tube[1], *(*args + (i + 1)));
	term_reset();
	ft_retab(*args, i + 1);
	ft_retab(*args, i);
	close(tube[1]);
	if (g_pipe->in == 1)
		dup2(old_dup, 1) == -1 ? dup_error() : 1;
}

int		apply_heredoc(char ***args, int tube[], int i)
{
	if (!*(*args + (i + 1)))
	{
		parse_error();
		return (0);
	}
	if (tube[0] != 0)
	{
		ft_retab(*args, i + 1);
		ft_retab(*args, i);
	}
	else
		do_heredoc(args, tube, i);
	return (1);
}

int			get_heredoc(char ***args, int tube[])
{
	int i;

	i = 0;
	while (*(*args + i))
	{
		if (ft_strcmp(*(*args + i), "<<") == 0)
		{
			if (!apply_heredoc(args, tube, i))
				return (0);
		}
		if (g_data->error == 1)
		{
			print_prompt(g_data->cpy);
			return (0);
		}
		i++;
	}
	return (1);
}

void	redir_sleftend(int *fd, int tube[])
{
	if (g_input->std0 != 0)
		dup2(g_input->std0, 0) == -1 ? dup_error() : 1;
	g_input->std0 = dup(0);
	dup2(*fd, 0) == -1 ? dup_error() : 1;
	g_input->op = 1;
	if (tube[0] != 0)
	{
		close(tube[0]);
		tube[0] = 0;
	}
}

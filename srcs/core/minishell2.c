/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:24:19 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/29 12:37:50 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	just_apply2(char ***args, int *fd)
{
	if (*(*args + 0) && ft_strcmp(*(*args + 0), "echo") == 0)
		ft_echo(*args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "cd") == 0)
		ft_cd(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "setenv") == 0)
		ft_setenv(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "unsetenv") == 0)
		ft_unsetenv(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "env") == 0)
		ft_env(&g_data->cpy, *args);
	else if (*(*args + 0) && ft_strcmp(*(*args + 0), "history") == 0)
		ft_history(*args);
	else
		ft_execve(*args, g_data->cpy);
	if (g_input->op != 0 && *fd > 0 && *fd != 0 && *fd != 1 && *fd != 2)
		close(*fd);
}

void	just_apply(char ***args)
{
	int i;
	int fd;

	i = 0;
	if ((fd = ft_redir(args)) == -1)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		return ;
	}
	while (*(*args + i))
	{
		*(*args + i) = args_translate(*(*args + i), *args);
		i++;
	}
	if (*(*args + 0) && ft_strcmp(*(*args + 0), "exit") == 0)
	{
		dup_std();
		if (g_input->op != 0 && fd > 0 && fd != 0 && fd != 1 && fd != 2)
			close(fd);
		return ;
	}
	just_apply2(args, &fd);
}

int		count_pipes(char **args)
{
	int		count;
	int		i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			count = count + 2;
		i++;
	}
	return (count);
}

t_nrm	*apply_init(int tube[], int **o_pid, char ***args, char ***arg)
{
	t_nrm *nrm;

	if (!(g_pipe = (t_pipe *)malloc(sizeof(t_pipe))))
		malloc_error();
	if (!(nrm = (t_nrm *)malloc(sizeof(t_nrm))))
		malloc_error();
	if (!(*o_pid = (int *)malloc(sizeof(int) * count_pipes(*arg))))
		malloc_error();
	if (!(*args = (char **)malloc(sizeof(char *) * tab_size(*arg) + 1)))
		malloc_error();
	nrm->count = 0;
	g_pipe->pid = 0;
	nrm->i = 0;
	nrm->j = 0;
	tube[0] = 0;
	tube[1] = 0;
	nrm->std = dup(0);
	nrm->std1 = dup(1);
	g_input->std0 = nrm->std;
	g_input->std1 = nrm->std1;
	return (nrm);
}

void	process_pipe(char ***args)
{
	int		tunnel[2];

	pipe(tunnel) == -1 ? pipe_error() : 1;
	if (!fork())
	{
		close(tunnel[0]);
		g_pipe->in = 1;
		just_apply(args);
		dup_std();
		write(tunnel[1], &g_pipe->pid, sizeof(int));
		close(tunnel[1]);
		exit(0);
	}
	else
	{
		close(tunnel[1]);
		wait(NULL);
		read(tunnel[0], &g_pipe->pid, sizeof(int));
		close(tunnel[0]);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:27:18 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 11:15:18 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		do_pipes(int tube[], t_nrm **nrm, char ***args, int **old_pids)
{
	if (ft_strcmp((*args)[(*nrm)->j], "|") == 0)
	{
		pipe(tube) == -1 ? pipe_error() : 1;
		dup2(tube[1], 1);
		free((*args)[(*nrm)->j]);
		if (g_pipe->pid != 0)
		{
			(*old_pids)[(*nrm)->count] = g_pipe->pid;
			(*nrm)->count++;
			g_pipe->pid = 0;
		}
		(*args)[(*nrm)->j] = NULL;
		process_pipe(args);
		ft_tabdel(args);
		(*nrm)->j = -1;
		dup2(tube[0], 0);
		close(tube[1]);
	}
}

static void		end_apply(char ***args, int **old_pids, t_nrm **nrm)
{
	dup2((*nrm)->std1, 1);
	g_pipe->in = 0;
	just_apply(args);
	dup_std();
	(*args)[tab_size(*args)] = NULL;
	(*old_pids)[(*nrm)->count] = 0;
	(*nrm)->count = 0;
	while ((*old_pids)[(*nrm)->count] != 0)
	{
		kill((*old_pids)[(*nrm)->count], SIGTERM);
		(*nrm)->count++;
	}
	dup2((*nrm)->std, 0);
	free(g_pipe);
	free(*nrm);
	free(*old_pids);
	ft_tabdel(args);
	free(*args);
}

void			ft_apply(char **arg)
{
	int		tube[2];
	char	**args;
	int		*old_pids;
	t_nrm	*nrm;

	nrm = apply_init(tube, &old_pids, &args, &arg);
	while (arg[nrm->i])
	{
		args[nrm->j] = ft_strdup(arg[nrm->i]);
		do_pipes(tube, &nrm, &args, &old_pids);
		nrm->j++;
		nrm->i++;
	}
	args[nrm->j] = NULL;
	if (g_pipe->pid != 0)
	{
		old_pids[nrm->count] = g_pipe->pid;
		nrm->count++;
		g_pipe->pid = 0;
	}
	end_apply(&args, &old_pids, &nrm);
}

void			flag_init(struct termios *term)
{
	term->c_lflag &= ~(ISIG);
	term->c_lflag &= ~(ICANON);
	term->c_lflag &= ~(ECHO);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 0;
}

void			termios_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("21sh", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": cant initialize struct termios\n", 2);
}

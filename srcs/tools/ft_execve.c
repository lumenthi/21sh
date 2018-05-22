/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:12:13 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/18 16:01:24 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/21sh.h"

static int	find_path(char **arg, char **env)
{
	char	*path;
	char	*cmd;

	path = get_var(env, "PATH=");
	path = ft_strjoin(path, "/");
	if (arg[0][0] != '/')
		cmd = ft_strjoin(path, arg[0]);
	else
		cmd = ft_strdup(arg[0]);
//	dprintf(2, "acces ret: %d\n", access(cmd, X_OK));
	free(path);
	free(cmd);
	return (access(cmd, X_OK));
}

static int	do_execve(char **arg, char **env)
{
	pid_t	pid;
	char	*cmd;
	char	*path;
	int		status;

	path = get_var(env, "PATH=");
	path = ft_strjoin(path, "/");
	if (arg[0][0] != '/')
		cmd = ft_strjoin(path, arg[0]);
	else
		cmd = ft_strdup(arg[0]);
	((pid = new_process()) == -1) ? exit(EXIT_FAILURE) : 9;
	if (pid == 0)
	{
		execve(cmd, arg, env);
		exit(3);
	}
	else
	{
		if (!in_pipe)
			waitpid(-1, &status, 0);
		else
			return (pid);
		free(path);
		free(cmd);
		return (status);
	}
}

static int	error_exec(char ***arg, char **env, char **fullpath, char **bu)
{
	if (tab_size(*arg) == 0)
		return (1);
	if (!get_var(env, "PATH="))
	{
		ft_print_error(*arg[0], VAR_FOUND, "$PATH");
		return (1);
	}
	*fullpath = ft_strdup(get_var(env, "PATH="));
	*bu = ft_strdup(*fullpath);
	free(*fullpath);
	*fullpath = ft_strjoin(*bu, ":");
	return (0);
}

static void	make_fullpath(char **fullpath, char **old)
{
	*old = ft_strdup(*fullpath);
	free(*fullpath);
	*fullpath = ft_strdup(ft_strchr(*old, ':'));
	free(*old);
	free(*fullpath);
	*fullpath = ft_strdup(*fullpath + 1);
}

static void	end_execve(char *path, char **arg, char ***env, char **bu)
{
	if (!path && g_data->error == 0)
		ft_print_error(arg[0], FT_FOUND, "");
	set_var(env, "PATH=", *bu);
	free(*bu);
}

void		ft_execve(char **arg, char **env)
{
	char	*fullpath;
	char	*path;
	char	*bu;
	char	*old;

	if (arg[0] && arg[0][0] == '/')
	{
		if (find_path(arg, env) != 0)
		{
			ft_print_error(arg[0], FT_FOUND, NULL);
			return ;
		}
		if (in_pipe)
			pipe_pid = do_execve(arg, env);
		else
			do_execve(arg, env);
		return ;
	}
	if (error_exec(&arg, env, &fullpath, &bu))
		return ;
	while ((path = make_string(fullpath)))
	{
		set_var(&env, "PATH=", path);
		free(path);
		if (find_path(arg, env) == 0)
		{
			if (in_pipe)
				pipe_pid = do_execve(arg, env);
			else
				do_execve(arg, env);
			break ;
		}
		make_fullpath(&fullpath, &old);
	}
	free(fullpath);
	end_execve(path, arg, &env, &bu);
}

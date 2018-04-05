/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:12:13 by lumenthi          #+#    #+#             */
/*   Updated: 2018/04/05 12:28:34 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	((pid = new_process()) == -1) ? exit(EXIT_FAILURE) : 0;
	if (pid == 0)
	{
		execve(cmd, arg, env);
		exit(42);
	}
	else
	{
		waitpid(-1, &status, WIFEXITED(status));
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
	int		ret;

	if (arg[0] && arg[0][0] == '/')
	{
		if (((ret = do_execve(arg, env))) && ret != 256 && g_data->error == 0)
			ft_print_error(arg[0], FT_FOUND, NULL);
		return ;
	}
	if (error_exec(&arg, env, &fullpath, &bu))
		return ;
	while ((path = make_string(fullpath)))
	{
		set_var(&env, "PATH=", path);
		free(path);
		if ((!(ret = do_execve(arg, env))) || ret == 256)
			break ;
		make_fullpath(&fullpath, &old);
	}
	free(fullpath);
	end_execve(path, arg, &env, &bu);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:12:13 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:17:21 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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

static int	absolute_exec(char ***arg, char ***env)
{
	if ((*arg)[0] && (*arg)[0][0] == '/')
	{
		if (find_path(*arg, *env) != 0)
		{
			ft_print_error((*arg)[0], FT_FOUND, NULL);
			return (1);
		}
		if (g_pipe->in)
			g_pipe->pid = do_execve(*arg, *env);
		else
			do_execve(*arg, *env);
		return (1);
	}
	return (0);
}

void		ft_execve(char **arg, char **env)
{
	char	*fullpath;
	char	*path;
	char	*bu;
	char	*old;

	if ((absolute_exec(&arg, &env)) || (error_exec(&arg, env, &fullpath, &bu)))
		return ;
	while ((path = make_string(fullpath)))
	{
		set_var(&env, "PATH=", path);
		free(path);
		if (find_path(arg, env) == 0)
		{
			if (g_pipe->in)
				g_pipe->pid = do_execve(arg, env);
			else
				do_execve(arg, env);
			break ;
		}
		make_fullpath(&fullpath, &old);
	}
	free(fullpath);
	end_execve(path, arg, &env, &bu);
}

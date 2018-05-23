/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execve2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:14:53 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/23 16:17:43 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		find_path(char **arg, char **env)
{
	char	*path;
	char	*cmd;

	path = get_var(env, "PATH=");
	path = ft_strjoin(path, "/");
	if (arg[0][0] != '/')
		cmd = ft_strjoin(path, arg[0]);
	else
		cmd = ft_strdup(arg[0]);
	free(path);
	free(cmd);
	return (access(cmd, X_OK));
}

void	get_cmd(char **path, char **env, char **cmd, char **arg)
{
	*path = get_var(env, "PATH=");
	*path = ft_strjoin(*path, "/");
	if (arg[0][0] != '/')
		*cmd = ft_strjoin(*path, arg[0]);
	else
		*cmd = ft_strdup(arg[0]);
}

int		do_execve(char **arg, char **env)
{
	pid_t	pid;
	char	*cmd;
	char	*path;
	int		status;

	get_cmd(&path, env, &cmd, arg);
	((pid = new_process()) == -1) ? exit(EXIT_FAILURE) : 9;
	if (pid == 0)
	{
		execve(cmd, arg, env);
		exit(3);
	}
	else
	{
		if (!g_pipe->in)
			waitpid(-1, &status, 0);
		else
			return (pid);
		free(path);
		free(cmd);
		return (status);
	}
}

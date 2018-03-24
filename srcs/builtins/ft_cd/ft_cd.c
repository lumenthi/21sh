/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/15 16:39:55 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/13 12:05:04 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char		*var_conv(char *arg, char **env)
{
	char	*str;
	char	*tmp;
	char	*ret;

	str = ft_strdup(arg + 1);
	tmp = ft_strjoin(str, "=");
	free(str);
	str = ft_strdup(tmp);
	free(tmp);
	ret = get_var(env, str);
	free(str);
	return (ret);
}

static void	get_paths(char **abs_path, char **old_path, char **environ)
{
	if (get_var(environ, "OLDPWD=") == NULL)
		*old_path = ft_strdup("");
	else
		*old_path = ft_strdup(get_var(environ, "OLDPWD="));
	*abs_path = NULL;
	*abs_path = getcwd(*abs_path, 99);
}

void		ft_cd(char ***environ, char **arg)
{
	char	*abs_path;
	char	*old_path;

	get_paths(&abs_path, &old_path, *environ);
	if (tab_size(arg) > 2)
		ft_print_error("cd", ARGS, NULL);
	else if (arg[1] != NULL && arg[1][0] == '$')
		cd_var(arg, environ, &abs_path);
	else if (arg[1] && ft_strcmp(arg[1], "-") == 0)
		cd_old(environ, &abs_path, old_path);
	else if (arg[1] == NULL || ft_strcmp(arg[1], "~") == 0 ||
		ft_strcmp(arg[1], "--") == 0)
		cd_home(environ, &abs_path);
	else
		cd_path(arg, environ, &abs_path);
	free(abs_path);
	free(old_path);
}

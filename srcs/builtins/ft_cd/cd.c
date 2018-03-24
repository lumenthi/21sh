/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:51:16 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/13 12:04:39 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	cd_error(char *path)
{
	ft_putstr(RED);
	ft_putstr("cd");
	ft_putstr(BLANK);
	if (ft_strcmp(path, "$HOME") == 0)
		ft_putendl(": no $HOME variable");
	else if (ft_strcmp(path, "$OLDPWD") == 0)
		ft_putendl(": no $OLDPWD variable");
	else
	{
		ft_putstr(": no such file or directory: ");
		ft_putendl(path);
	}
}

void		cd_var(char **arg, char ***environ, char **abs_path)
{
	char *tmp;

	if (!(tmp = var_conv(arg[1], *environ)))
		ft_print_error("cd", VAR_FOUND, arg[1]);
	else if (chdir(tmp) == -1)
		cd_error(tmp);
	else
	{
		set_var(environ, "OLDPWD=", *abs_path);
		*abs_path = getcwd(*abs_path, 99);
		set_var(environ, "PWD=", *abs_path);
	}
}

void		cd_old(char ***environ, char **ap, char *op)
{
	if (chdir(op) == -1)
		cd_error("$OLDPWD");
	else
	{
		set_var(environ, "OLDPWD=", *ap);
		*ap = getcwd(*ap, 99);
		set_var(environ, "PWD=", *ap);
		ft_putendl(*ap);
	}
}

void		cd_home(char ***environ, char **abs_path)
{
	if (!get_var(*environ, "HOME="))
		ft_print_error("cd", VAR_FOUND, "$HOME");
	else if (chdir(get_var(*environ, "HOME=")) == -1)
		cd_error(get_var(*environ, "HOME="));
	else
	{
		set_var(environ, "OLDPWD=", *abs_path);
		*abs_path = getcwd(*abs_path, 99);
		set_var(environ, "PWD=", *abs_path);
	}
}

void		cd_path(char **arg, char ***environ, char **abs_path)
{
	if (chdir(arg[1]) == -1)
		cd_error(arg[1]);
	else
	{
		set_var(environ, "OLDPWD=", *abs_path);
		*abs_path = getcwd(*abs_path, 99);
		set_var(environ, "PWD=", *abs_path);
	}
}

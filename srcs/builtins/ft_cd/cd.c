/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:51:16 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/03 12:17:51 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static void	cd_error(char *path)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("cd", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putstr_fd(": no such file or directory: ", 2);
	ft_putendl_fd(path, 2);
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

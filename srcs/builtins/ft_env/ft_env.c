/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:08:43 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/14 18:22:05 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

static int		set_env(char ***environ, char *env)
{
	char	*key;
	char	*value;
	char	*tmp;

	key = ft_strdup(env);
	tmp = ft_strchr(key, '=') + 1;
	value = ft_strdup(tmp);
	*tmp = '\0';
	set_var(environ, key, value);
	if (key[0] == '=')
	{
		free(key);
		free(value);
		return (0);
	}
	free(key);
	free(value);
	return (1);
}

static void		print_environ(char ***environ)
{
	int i;

	i = 0;
	while (*(*environ + i))
	{
		ft_putendl(*(*environ + i));
		i++;
	}
	if (i == 0)
		ft_print_error("env", EMPTY, NULL);
}

static int		make_fake(char ***fake_env, char **args, int *j)
{
	while (args[*j])
	{
		if (ft_strchr(args[*j], '='))
		{
			if (!set_env(fake_env, args[*j]))
			{
				ft_print_error("env", ARGS, NULL);
				return (0);
			}
			(*j)++;
		}
		else
			break ;
	}
	return (*j);
}

static void		reset_env(char ***environ, char ***bu, char ***fake_env)
{
	ft_tabdel(environ);
	free(*environ);
	environ_cpy(*bu, environ);
	ft_tabdel(fake_env);
	free(*fake_env);
	ft_tabdel(bu);
	free(*bu);
}

static void		just_env(int j, char **args)
{
	int i;

	i = 0;
	while ((args + j)[i])
	{
		(args + j)[i] = args_translate((args + j)[i], (args + j));
		i++;
	}
	if (ft_strcmp(args[1], "exit") != 0 && ft_strcmp(args[1], "q") != 0)
		ft_apply(args + j);
	else
		env_error();
}

void			ft_env(char ***environ, char **args)
{
	int		j;
	char	**fake_env;
	char	**bu;
	int		i;

	environ_cpy(*environ, &fake_env);
	environ_cpy(*environ, &bu);
	i = 0;
	j = 1;
	if (!(j = make_fake(&fake_env, args, &j)))
	{
		reset_env(environ, &bu, &fake_env);
		return ;
	}
	fake_cpy(environ, fake_env);
	if (j == tab_size(args))
		print_environ(environ);
	else
		just_env(j, args);
	reset_env(environ, &bu, &fake_env);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 14:08:43 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/20 13:55:37 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
		return (0);
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

static void		make_fake(char ***fake_env, char **args, int *j, char **line)
{
	while (args[*j])
	{
		if (ft_strchr(args[*j], '='))
		{
			if (!set_env(fake_env, args[*j]))
			{
				ft_print_error("env", ARGS, NULL);
				return ;
			}
		}
		else
			break ;
		*line = *line + ft_strlen(args[*j]) + 1;
		(*j)++;
	}
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

void			ft_env(char ***environ, char **args, char **line)
{
	int		j;
	char	**fake_env;
	char	**bu;
	char	*line_cpy;

	environ_cpy(*environ, &fake_env);
	environ_cpy(*environ, &bu);
	j = 1;
	line_cpy = *line + 3;
	make_fake(&fake_env, args, &j, &line_cpy);
	fake_cpy(environ, fake_env);
	if (j == tab_size(args))
		print_environ(environ);
	else
	{
		if (ft_strcmp(args[1], "exit") != 0 && ft_strcmp(args[1], "q") != 0)
		{
			line_cpy = ft_strdup(line_cpy);
			ft_minishell(&line_cpy);
		}
		else
			env_error();
	}
	reset_env(environ, &bu, &fake_env);
}

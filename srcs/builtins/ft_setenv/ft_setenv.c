/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/16 10:15:47 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/07 12:02:41 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	rep_var(char ***environ, char *var, char *value)
{
	char	*str;
	int		i;
	char	*env;

	i = 0;
	while (*(*environ + i))
	{
		env = ft_strdup(*(*environ + i));
		str = ft_strchr(env, '=');
		*(str + 1) = '\0';
		if (!(ft_strcmp(env, var)))
		{
			free(*(*environ + i));
			free(env);
			break ;
		}
		free(env);
		i++;
	}
	value = ft_strjoin(var, value);
	*(*environ + i) = ft_strdup(value);
	free(value);
}

char	**new_env(char **cpy, char **args)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	while (*(cpy + i))
		i++;
	cpy = (char **)ft_realloc(cpy, sizeof(char*) * (i + 2));
	tmp = ft_strdup(args[1]);
	tmp2 = ft_strjoin(tmp, "=");
	free(tmp);
	free(args[1]);
	args[1] = ft_strjoin(tmp2, args[2]);
	free(tmp2);
	*(cpy + i) = ft_strdup(args[1]);
	i++;
	*(cpy + i) = NULL;
	return (cpy);
}

void	set_var(char ***cpy, char *var, char *value)
{
	char	**args;

	args = malloc(sizeof(char *) * 4);
	args[0] = ft_strdup("setenv");
	args[1] = ft_strdup(var);
	args[1][ft_strlen(args[1]) - 1] = '\0';
	args[2] = ft_strdup(value);
	args[3] = NULL;
	if (get_var(*cpy, var) != NULL)
		rep_var(cpy, var, value);
	else
		*cpy = new_env(*cpy, args);
	ft_tabdel(&args);
	free(args);
}

void	ft_setenv(char ***cpy, char **args)
{
	char	*tmp;

	if (args == NULL || tab_size(args) != 3)
	{
		ft_print_error("setenv", ARGS, NULL);
		return ;
	}
	else if (get_var(*cpy, tmp = ft_strjoin(args[1], "=")) != NULL)
		rep_var(cpy, tmp, args[2]);
	else
		*cpy = new_env(*cpy, args);
	free(tmp);
}

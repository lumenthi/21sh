/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections9.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:04:10 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 22:07:32 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**after_pipes(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	found = ft_strchr(before, '|');
	after = ft_strdup(found + 1);
	sign = ft_strdup(found);
	sign[1] = '\0';
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	free(after);
	free(sign);
	free(before);
	return (args);
}

char	**resize_pipes(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	before = ft_strdup(args[i]);
	found = ft_strchr(before, '|');
	if (*(found + 1) == '|')
	{
		parse_error();
		return (NULL);
	}
	after = ft_strdup(found + 1);
	sign = ft_strdup(found);
	sign[1] = '\0';
	*found = '\0';
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	args = tab_insert(args, i + 2, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

int		get_pipefound(char **found, char **cpy, int i, char **args)
{
	if (*(*found + 1) == '|' || *(*found + 1) == '<' ||
		*(*found + 1) == '>')
	{
		parse_error();
		free(*cpy);
		return (0);
	}
	if (ft_strlen(args[i]) != ft_strlen(*found))
	{
		if (*(*found - 1) == '<' || *(*found - 1) == '>')
		{
			parse_error();
			free(*cpy);
			return (0);
		}
	}
	return (1);
}

void	change_args(char ***args, int i, char **after, char *cpy)
{
	if (ft_strcmp(cpy, "") == 0)
		*args = before_pipes(*args, i);
	else if (ft_strcmp(*after, "") == 0)
		*args = after_pipes(*args, i);
	else
		*args = resize_pipes(*args, i);
	free(*after);
}

char	**retab_pipes(char **args)
{
	int		i;
	char	*after;
	char	*found;
	char	*cpy;

	i = 0;
	while (args[i])
	{
		cpy = ft_strdup(args[i]);
		if ((found = strchr_quote(cpy, '|')) && ft_strlen(args[i]) != 1)
		{
			if (!get_pipefound(&found, &cpy, i, args))
				return (NULL);
			after = ft_strdup(found + 1);
			*found = '\0';
			change_args(&args, i, &after, cpy);
		}
		free(cpy);
		i++;
	}
	return (args);
}

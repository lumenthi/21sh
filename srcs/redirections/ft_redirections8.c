/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections8.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:59:57 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 22:03:02 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		retab_dirsinit(int *i, int *ret)
{
	*i = 0;
	*ret = 0;
}

void		loop_forward(int *i, char **cpy)
{
	free(*cpy);
	(*i)++;
}

void		retab_norm(char **after, char **found)
{
	*after = ft_strdup(*found + 1);
	**found = '\0';
}

char			**retab_dirs(char **args)
{
	int		i;
	char	*after;
	char	*found;
	char	*cpy;
	int		ret;

	retab_dirsinit(&i, &ret);
	while (args[i])
	{
		cpy = ft_strdup(args[i]);
		if (retab_condition(&found, cpy, args[i]))
		{
			if ((ret = arg_last_redir(&args[i], &cpy)) == -1)
				return (NULL);
			else if (ret != 0)
			{
				retab_norm(&after, &found);
				if (!retab_argsdir(&args, &cpy, &after, i))
					return (NULL);
			}
		}
		loop_forward(&i, &cpy);
	}
	return (args);
}

char	**before_pipes(char **args, int i)
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
	args[i] = ft_strdup(sign);
	args = tab_insert(args, i + 1, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

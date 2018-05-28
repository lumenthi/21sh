/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections7.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:51:23 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 10:57:22 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	get_found(char **found, char **after, char *cpy, char **str)
{
	if (!(*found = strrchr_quote(cpy, '<')))
		*found = strrchr_quote(cpy, '>');
	if (*(*found - 1) == '>' || *(*found - 1) == '<')
		*found = *found - 1;
	*after = ft_strdup(*found);
	if (!(*found = ft_strchr(cpy, '<')))
		*found = ft_strchr(cpy, '>');
	**found = '\0';
	free(*str);
	*str = ft_strjoin(cpy, *after);
}

int		arg_last_redir(char **str, char **cpy2)
{
	char	*found;
	char	*after;
	char	*cpy;

	if (!(valid_redir(*str)))
	{
		free(*cpy2);
		parse_error();
		return (-1);
	}
	cpy = ft_strdup(*str);
	get_found(&found, &after, cpy, str);
	if (ft_strchr(after, '&'))
	{
		free(after);
		free(cpy);
		return (0);
	}
	free(after);
	free(cpy);
	return (1);
}

void	nb_retabs(char ***args, int i, char *after)
{
	char	*final;

	if (ft_strcmp(after, "") == 0 || ft_strcmp(after, ">") == 0
	|| ft_strcmp(after, "<") == 0)
	{
		*args = nb_lastredir(*args, i);
		final = ft_strjoin(*(*args + i), *(*args + i + 1));
		free(*(*args + i));
		*(*args + i) = ft_strdup(final);
		ft_retab(*args, i + 1);
		free(final);
	}
	*args = nb_lastredir(*args, i);
	final = ft_strdup(*(*args + i));
	free(*(*args + i));
	*(*args + i) = remove_quote(final);
	free(final);
}

int		retab_argsdir(char ***args, char **cpy, char **after, int i)
{
	if (!*(*args + i + 1) && (ft_strcmp(*after, "") == 0 ||
	ft_strcmp(*after, ">") == 0 || ft_strcmp(*after, "<") == 0))
	{
		parse_error();
		free(*cpy);
		free(*after);
		return (0);
	}
	if (ft_isnum(*cpy) && ft_strcmp(*cpy, "") != 0)
		nb_retabs(args, i, *after);
	else if (ft_strcmp(*cpy, "") == 0)
		*args = before_resize(*args, i);
	else if (ft_strcmp(*after, "") == 0 ||
		ft_strcmp(*after, ">") == 0 || ft_strcmp(*after, "<") == 0)
		*args = after_resize(*args, i);
	else
		*args = tab_resize(*args, i);
	free(*after);
	return (1);
}

int		retab_condition(char **found, char *cpy, char *args)
{
	if (!opin_quote(cpy) && ((*found = strchr_quote(cpy, '>')) ||
		(*found = strchr_quote(cpy, '<'))) &&
		ft_strcmp(cpy, ">>") != 0 && (ft_strcmp(cpy, "<<") != 0) &&
		ft_strlen(args) != 1)
		return (1);
	return (0);
}

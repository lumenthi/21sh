/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:35:06 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 21:36:54 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**tab_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	get_afterredir(&after, &found, &sign);
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	args = tab_insert(args, i + 2, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

char	**before_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	get_afterredir(&after, &found, &sign);
	free(args[i]);
	args[i] = ft_strdup(sign);
	args = tab_insert(args, i + 1, after);
	free(after);
	free(sign);
	free(before);
	return (args);
}

char	**after_resize(char **args, int i)
{
	char	*before;
	char	*after;
	char	*found;
	char	*sign;

	found = NULL;
	before = ft_strdup(args[i]);
	if (!(found = ft_strchr(before, '>')))
		found = ft_strchr(before, '<');
	get_afterredir(&after, &found, &sign);
	free(args[i]);
	args[i] = ft_strdup(before);
	args = tab_insert(args, i + 1, sign);
	free(after);
	free(sign);
	free(before);
	return (args);
}

void	get_sign(char **found, char *line, char **sign)
{
	**found = '\0';
	if ((*found = strchr_quote(line, '>')))
	{
		if (*(--*found) == '>')
			*sign = ft_strdup(">>");
		else
			*sign = ft_strdup(">");
		++*found;
	}
	else
	{
		*found = strchr_quote(line, '<');
		if (*(--*found) == '<')
			*sign = ft_strdup("<<");
		else
			*sign = ft_strdup("<");
		++*found;
	}
}

char	**nb_lastredir(char **args, int i)
{
	char	*found;
	char	*line;
	char	*before;
	char	*sign;

	sign = NULL;
	line = ft_strdup(args[i]);
	before = ft_strdup(args[i]);
	if (!(found = strchr_quote(before, '>')))
		found = strchr_quote(before, '<');
	if (!found)
		return (args);
	get_sign(&found, line, &sign);
	free(args[i]);
	args[i] = ft_strjoin(before, sign);
	free(before);
	before = ft_strdup(args[i]);
	free(args[i]);
	args[i] = ft_strjoin(before, found + 1);
	if (sign)
		free(sign);
	free(line);
	free(before);
	return (args);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 21:09:50 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 21:11:41 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		ft_isnum(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	last_redir(char **after)
{
	char *found;

	found = NULL;
	if (!(found = ft_strrchr(*after, '>')))
		found = ft_strrchr(*after, '<');
	if (!found)
		;
	else
	{
		free(*after);
		*after = ft_strdup(found + 1);
	}
}

int		heredoc_nb(char *cpy, char *found)
{
	char	*after;
	int		tube[2];

	*found = '\0';
	after = ft_strdup(found + 2);
	pipe(tube) == -1 ? pipe_error() : 1;
	term_init();
	write_mode(tube[1], after);
	term_reset();
	close(tube[1]);
	if (ft_atoi(cpy) == 0)
		g_input->std0 = dup(ft_atoi(cpy));
	else if (ft_atoi(cpy) == 1)
		g_input->std1 = dup(ft_atoi(cpy));
	else if (ft_atoi(cpy) == 2)
		g_input->std2 = dup(ft_atoi(cpy));
	dup2(tube[0], ft_atoi(cpy)) == -1 ? dup_error() : 1;
	g_input->op = 1;
	free(after);
	return (tube[0]);
}

char	*get_after(char **found, char **cpy, int *sign, int *new_fd)
{
	char	*after;

	*sign = 1;
	if (**found == '<')
	{
		*sign = -1;
		if (*(*found + 1) == '<')
		{
			*sign = 2;
			*new_fd = heredoc_nb(*cpy, *found);
			free(*cpy);
			return (NULL);
		}
		after = ft_strdup(*found + 1);
	}
	else if (*(*found + 1) == '>')
	{
		after = ft_strdup(*found + 2);
		*sign = 2;
	}
	else
		after = ft_strdup(*found + 1);
	**found = '\0';
	return (after);
}

void	not_filefd(int *new_fd, int sign, char *after)
{
	if (sign == 0)
		*new_fd = open(after, O_RDONLY);
	else if (sign == 1)
		*new_fd = open(after, O_RDWR|O_CREAT|O_TRUNC, 0666);
	else
		*new_fd = open(after, O_RDWR|O_CREAT|O_APPEND, 0666);
}

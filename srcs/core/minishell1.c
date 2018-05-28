/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:21:27 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 11:14:16 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	dup_std(void)
{
	if (g_input->std0 != 0)
		dup2(g_input->std0, 0);
	if (g_input->std1 != 0)
		dup2(g_input->std1, 1);
	if (g_input->std2 != 0)
		dup2(g_input->std2, 2);
}

void	retab_init(char ***args, int *j, char **tmp, int i)
{
	*j = 0;
	while (*j <= i)
	{
		*tmp = ft_strdup(*(*args + *j));
		free(*(*args + *j));
		*(*args + *j) = ft_strdup(*tmp);
		free(*tmp);
		(*j)++;
	}
}

void	ft_retab(char **args, int i)
{
	int		j;
	char	*tmp;

	retab_init(&args, &j, &tmp, i);
	while (args[i])
	{
		if (args[i + 1])
		{
			free(args[i]);
			args[i] = ft_strdup(args[i + 1]);
		}
		else
		{
			free(args[i]);
			args[i] = NULL;
			break ;
		}
		i++;
	}
}

void	inputs_init(void)
{
	if (!(g_input = malloc(sizeof(g_input))))
		malloc_error();
	g_input->std0 = 0;
	g_input->std1 = 0;
	g_input->std2 = 0;
	g_input->op = 0;
}

void	inputs_reset(void)
{
	free(g_input);
}

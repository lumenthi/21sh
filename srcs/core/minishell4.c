/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 22:29:30 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 15:00:49 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		valid_term(void)
{
	char	*name_term;

	name_term = get_var(g_data->cpy, "TERM=");
	if (!name_term)
	{
		termios_error();
		return (0);
	}
	return (1);
}

void	term_init(void)
{
	char			*name_term;
	struct termios	term;

	if (!(g_data->bu = malloc(sizeof(struct termios))))
		malloc_error();
	name_term = get_var(g_data->cpy, "TERM=");
	tgetent(NULL, name_term);
	tcgetattr(0, g_data->bu);
	tcgetattr(0, &term);
	flag_init(&term);
	tcsetattr(0, TCSADRAIN, &term);
}

void	term_reset(void)
{
	if (g_data->bu != NULL)
	{
		tcsetattr(0, 0, g_data->bu);
		free(g_data->bu);
		g_data->bu = NULL;
	}
}

int		del_args(char ***arg)
{
	if (*arg)
	{
		ft_tabdel(arg);
		free(*arg);
	}
	return (1);
}

int		ft_minishell(char **line)
{
	char	**args;
	char	**arg;

	args = NULL;
	term_init();
	quote_get2(line);
	term_reset();
	args = get_a(*line, args);
	if (!(retab_all(&args, &arg)))
	{
		free(*line);
		return (0);
	}
	else
		ft_apply(arg);
	if (arg[0] && ft_strcmp(arg[0], "exit") == 0)
	{
		del_args(&args);
		return (1);
	}
	del_args(&arg);
	free(*line);
	return (0);
}

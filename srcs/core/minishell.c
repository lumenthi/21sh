/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:24:59 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 10:50:06 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

static int		do_minishell(char **str, char **base, char **tmp)
{
	if (ft_minishell(str))
	{
		free(*base);
		free(*str);
		return (1);
	}
	free(*tmp);
	return (0);
}

int				ft_commands(char **line)
{
	char	*str;
	char	*found;
	char	*base;
	char	*tmp;

	base = ft_strdup(*line);
	str = ft_strdup(*line);
	while ((found = strchr_quote(str, ';')))
	{
		loop_init(&found, &str, &base, &tmp);
		if (do_minishell(&str, &base, &tmp))
			return (1);
		if (g_data->error)
		{
			free(*line);
			free(base);
			return (0);
		}
		str = ft_strdup(base);
	}
	return (end_commands(&str, &base, line));
}

void			set_lvl(void)
{
	int		nb;
	char	*line;

	line = get_var(g_data->cpy, "SHLVL=");
	if (line)
	{
		nb = ft_atoi(line) + 1;
		line = ft_itoa(nb);
	}
	else
		line = ft_strdup("1");
	set_var(&g_data->cpy, "SHLVL=", line);
	free(line);
}

static int		ft_shell(void)
{
	g_data->line = NULL;
	get_winsize();
	term_init();
	if (!g_data->error)
		print_prompt(g_data->cpy);
	g_data->line = gnl();
	term_reset();
	g_data->error = 0;
	if (g_data->line)
	{
		write_file();
		if (ft_commands(&g_data->line))
			return (1);
	}
	return (0);
}

int				main(void)
{
	extern char	**environ;

	history_init();
	data_init();
	inputs_init();
	g_data->error = 0;
	environ_cpy(environ, &g_data->cpy);
	all_signals();
	valid_term();
	set_lvl();
	while (1)
	{
		if (ft_shell())
			break ;
	}
	inputs_reset();
	data_free();
	history_free();
	return (0);
}

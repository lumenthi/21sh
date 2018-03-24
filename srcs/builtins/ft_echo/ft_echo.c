/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 19:00:43 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/08 11:53:32 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_echo(char **args, char **cpy)
{
	int		i;
	char	*var;
	int		f;

	i = 1;
	f = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "~") == 0)
			get_var(cpy, "HOME=") ? ft_putstr(get_var(cpy, "HOME=")) : f++;
		else if (args[i][0] == '$')
		{
			var = ft_strjoin(args[i] + 1, "=");
			get_var(cpy, var) ? ft_putstr(get_var(cpy, var)) : f++;
			free(var);
		}
		else
			ft_putstr(args[i]);
		if (args[i + 1] && f == 0)
			ft_putchar(' ');
		f = 0;
		i++;
	}
	ft_putchar('\n');
}

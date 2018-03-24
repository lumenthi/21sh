/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:44:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/20 13:54:25 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	env_error(void)
{
	ft_putstr(RED);
	ft_putstr("env");
	ft_putstr(BLANK);
	ft_putendl(": can't leave through env commad");
}

void	fake_cpy(char ***environ, char **fake_env)
{
	ft_tabdel(environ);
	free(*environ);
	environ_cpy(fake_env, environ);
}

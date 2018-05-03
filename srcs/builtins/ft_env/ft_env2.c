/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 13:44:34 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/03 12:18:38 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/21sh.h"

void	env_error(void)
{
	ft_putstr_fd(RED, 2);
	ft_putstr_fd("env", 2);
	ft_putstr_fd(BLANK, 2);
	ft_putendl_fd(": can't leave through env commad", 2);
}

void	fake_cpy(char ***environ, char **fake_env)
{
	ft_tabdel(environ);
	free(*environ);
	environ_cpy(fake_env, environ);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/28 14:59:02 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/28 15:00:06 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		retab_all(char ***args, char ***arg)
{
	if (!(*arg = retab_pipes(*args)))
		return (0);
	if (!(*arg = retab_dirs(*arg)))
		return (0);
	return (1);
}

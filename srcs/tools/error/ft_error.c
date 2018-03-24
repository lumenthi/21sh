/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 11:40:30 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/13 11:41:48 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	ft_print_error(char *ft, int error, char *arg)
{
	if (error == EMPTY)
		print_empty(ft);
	if (error == QUOTES)
		print_quotes(arg);
	if (error == FT_FOUND)
		print_ft_found(ft);
	if (error == ARGS)
		print_args(ft);
	if (error == VAR_FOUND)
		print_var_found(ft, arg);
}

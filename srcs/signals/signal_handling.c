/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/08 14:51:49 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 15:39:10 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		all_signals(void)
{
	signal(SIGABRT, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGALRM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGPIPE, signal_handler);
	signal(SIGFPE, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGCONT, signal_handler);
}

void		signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_data->error = 1;
		dup_std();
		ft_putstr("\b ");
		ft_putstr("\b\b ");
		ft_putstr("\b");
		ft_putchar('\n');
		print_prompt(g_data->cpy);
		if (g_data->line)
			free(g_data->line);
		g_data->line = ft_strdup("");
	}
	else if (sig == SIGABRT || sig == SIGQUIT || sig == SIGTERM
		|| sig == SIGALRM || sig == SIGHUP || sig == SIGPIPE || sig == SIGSEGV
		|| sig == SIGFPE || sig == SIGILL || sig == SIGUSR1 || sig == SIGUSR2)
	{
		term_reset();
		ft_putstr_fd("program exited with code: ", 2);
		ft_putnbr_fd(sig, 2);
		ft_putchar_fd('\n', 2);
		exit(-1);
	}
}

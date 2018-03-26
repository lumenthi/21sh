/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/03/25 14:31:08 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define GREEN "\033[1;32m"
# define BLUE "\033[4;34m"
# define BLANK "\033[0m"
# define RED "\033[1;31m"
# define RIGHT (buf[0] == 27 && buf[1] == 79 && buf[2] == 67)
# define LEFT (buf[0] == 27 && buf[1] == 79 && buf[2] == 68)
# define DOWN (buf[0] == 27 && buf[1] == 79 && buf[2] == 66)
# define UP (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
# define ECHAP (buf[0] == 27 && buf[1] == 0 && buf[2] == 0)

# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <termios.h>
# include <term.h>
# include <sys/ioctl.h>
# include <fcntl.h>

enum {ARGS, VAR_FOUND, FT_FOUND, QUOTES, EMPTY};

typedef struct		s_cursor
{
	int	x;
	int	y;
	int	start;
}					t_cursor;

typedef struct		s_history
{
	int	fd;
	int	position;
	int	nb_lines;
}					t_history;

typedef struct		s_data
{
	int				w_row;
	int				w_col;
	int				error;
	char			**cpy;
	struct termios	*bu;
	t_cursor		*cursor;
	char			*line;
}					t_data;

pid_t	new_process(void);
void	ft_tabdel(char ***tabs);
char	*get_var(char **environ, char *var);
void	set_var(char ***environ, char *var, char *value);
void	ft_cd(char ***environ, char **arg);
void	print_env(char **environ);
void	ft_unsetenv(char ***cpy, char **args);
void	ft_setenv(char ***cpy, char **args);
void	environ_cpy(char **environ, char ***cpy);
void	ft_env(char ***environ, char **arg, char **line);
void	ft_execve(char **arg, char **env);
void	ft_echo(char **tabs, char **cpy);
int		tab_size(char **tabs);
void	ft_print_error(char *ft, int error, char *arg);
char	*var_conv(char *arg, char **env);
char	**get_a(char *line, char **args);
void	print_empty(char *ft);
void	print_quotes(char *arg);
void	print_ft_found(char *ft);
void	print_args(char *ft);
void	print_var_found(char *ft, char *arg);
int		quote_invalid(char *line);
void	cd_var(char **arg, char ***environ, char **abs_path);
void	cd_old(char ***environ, char **ap, char *op);
void	cd_home(char ***environ, char **abs_path);
void	cd_path(char **arg, char ***environ, char **abs_path);
char	**tab_conv(char **args, char **env);
char	*make_string(char *fullpath);
void	print_prompt(char **cpy);
char	*gnl(void);
int		ft_minishell(char **line);
void	env_error(void);
void	fake_cpy(char ***environ, char **fake_env);
void	move_cursor(int x, int y);
void	get_winsize(void);
void	ft_put(char *str);
int		my_outc(int c);

t_data	*g_data;
t_history	*history;

#endif

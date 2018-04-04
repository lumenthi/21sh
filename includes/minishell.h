/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/04/04 11:18:47 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define BLANK "\033[0m"
# define RED "\033[1;31m"
# define ORANGE "\033[38;5;214m"
# define YELLOW "\033[38;5;227m"

#define HISTORY_LIMIT 15

# define RIGHT (buf[0] == 27 && buf[1] == 79 && buf[2] == 67)
# define LEFT (buf[0] == 27 && buf[1] == 79 && buf[2] == 68)
# define DOWN (buf[0] == 27 && buf[1] == 79 && buf[2] == 66)
# define UP (buf[0] == 27 && buf[1] == 79 && buf[2] == 65)
# define A_RIGHT (buf[0] == 27 && buf[1] == 27  && buf[2] == 91 && buf[3] == 67)
# define A_LEFT (buf[0] == 27 && buf[1] == 27  && buf[2] == 91 && buf[3] == 68)
# define A_UP (buf[0] == 27 && buf[1] == 27  && buf[2] == 91 && buf[3] == 65)
# define A_DOWN (buf[0] == 27 && buf[1] == 27  && buf[2] == 91 && buf[3] == 66)
# define A_C (buf[0] == -61 && buf[1] == -89 && buf[2] == 0)
# define ECHAP (buf[0] == 27 && buf[1] == 0 && buf[2] == 0)
# define HOME (buf[0] == 27 && buf[1] == 79 && buf[2] == 72)
# define END (buf[0] == 27 && buf[1] == 79 && buf[2] == 70)
# define BACKSPACE (buf[0] == 127 && buf[1] == 0 && buf[2] == 0)
# define SPACE (buf[0] == 32 && buf[1] == 0 && buf[2] == 0)
# define ENTER (buf[0] == 10 && buf[1] == 0 && buf[2] == 0)

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
# include <stdio.h>

enum {ARGS, VAR_FOUND, FT_FOUND, QUOTES, EMPTY};

typedef struct		s_cursor
{
	int	x;
	int	y;
	int	start;
}					t_cursor;

typedef struct		s_history
{
	int		position;
	int		nb_lines;
	char	**line;
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
	int				pos;
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
char	*quote_mode(char mode);
char	*insert_str(char *line, char *ins, int pos, int i);
char	*ft_delete(char *line, int pos, int i);

t_data	*g_data;
t_history	*history;

#endif

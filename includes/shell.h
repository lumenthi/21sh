/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/08 11:22:09 by lumenthi          #+#    #+#             */
/*   Updated: 2018/05/24 22:33:16 by lumenthi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define GREEN "\033[1;32m"
# define BLUE "\033[1;34m"
# define BLANK "\033[0m"
# define RED "\033[1;31m"
# define ORANGE "\033[38;5;214m"
# define YELLOW "\033[38;5;227m"

# define HISTORY_LIMIT 50
# define HISTORY_PATH "/tmp/.21sh_history"

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
# define CTRL_L (buf[0] == 12 && buf[1] == 0 && buf[2] == 0)
# define CTRL_C (buf[0] == 3 && buf[1] == 0 && buf[2] == 0)
# define CTRL_D (buf[0] == 4 && buf[1] == 0 && buf[2] == 0)

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

typedef struct		s_inputs
{
	int		std0;
	int		std1;
	int		std2;
	int		op;
}					t_inputs;

typedef struct		s_history
{
	int		position;
	int		nb_lines;
	char	**line;
	int		error;
	int		special;
}					t_history;

typedef struct		s_nrm
{
	int		i;
	int		j;
	int		std;
	int		std1;
	int		count;
}					t_nrm;

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

typedef struct		s_pipe
{
	int				in;
	int				pid;
}					t_pipe;

pid_t	new_process(void);
void	ft_tabdel(char ***tabs);
char	*get_var(char **environ, char *var);
void	set_var(char ***environ, char *var, char *value);
void	ft_cd(char ***environ, char **arg);
void	print_env(char **environ);
void	ft_unsetenv(char ***cpy, char **args);
void	ft_setenv(char ***cpy, char **args);
void	environ_cpy(char **environ, char ***cpy);
void	ft_env(char ***environ, char **arg);
void	ft_execve(char **arg, char **env);
void	ft_echo(char **tabs);
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
void	ft_apply(char **args);
void	env_error(void);
void	fake_cpy(char ***environ, char **fake_env);
void	move_cursor(int x, int y);
void	get_winsize(void);
void	ft_put(char *str);
int		my_outc(int c);
char	*quote_mode(char mode);
char	*insert_str(char *line, char *ins, int pos, int i);
void	write_mode(int fd, char *lim);
void	term_init(void);
void	term_reset(void);
char	*get_content(int fd);
char	*args_translate(char *line, char **args);
char	*strchr_quote(char *line, int elem);
void	ft_retab(char **args, int i);
char	**retab_dirs(char **args);
void	dup_std(void);
void	signal_handler(int sig);
void	all_signals(void);
void	ft_history(char **args);
void	history_free(void);
void	free_lines(void);
void	history_error(void);
void	fd_error(char *str);
void	parse_error(void);
void	input_error(char *line);
int		ft_redir(char ***arg);
void	quote_get2(char **file);
void	history_error(void);
void	history_init(void);
void	write_file(void);
void	history_search(int *i, char a);
void	ft_rewrite(int *i);
int		ft_move(char dir, int i);
void	edit_line(int *i);
void	ft_clear(int i);
void	inser_char(char buf, int *i);
void	ft_home(int i);
void	ft_end(int i);
void	line_up(int i);
void	line_down(int i);
void	word_left(int i);
void	word_right(int i);
void	copy_mode(int *i);
char	*remove_quote(char *line);
char	*strchr_quote(char *line, int elem);
int		valid_redir(char *str);
char	**retab_pipes(char **args);
void	just_apply(char ***args);
void	just_apply2(char ***args, int *fd);
void	print_environ(char ***environ);
int		dquotes_valid(char *line, int c);
void	get_normal(int *c, char *line, char **str, int *j);
int		history_open(int fd);
void	get_lines(void);
int		standard_enter(int i);
int		standard_sigint(int i);
int		standard_ctrld(int *i);
int		shortcut_clear(void);
int		shortcut_echap(int i);
void	copy_sub(int len, int *i);
void	copy_write(int pos, int *i);
void	mode_icon(char icon, int i);
int		copy_sigint(int *i);
void	copy_cpy(int *i, int *select);
void	copy_cut(int *i, char **cpy, int *select);
int		copy_paste(int *i, char **cpy, int *select);
int		copy_quit(int *i, int select);
int		quote_leave(int *i, char mode);
int		quote_ctrld(int *i);
void	ft_print(int len);
char	*ft_insert(char *line, char buf, int pos, int i);
int		find_path(char **arg, char **env);
void	get_cmd(char **path, char **env, char **cmd, char **arg);
int		do_execve(char **arg, char **env);
char	*home_translate(char *line, int i);
void	variables_init(int *i, char *mode);
void	translate_end(char **ne, char *line, char **args);
char	*var_translate(char *line, int i);
char	*point_translate(char *line, int i);
void	pipe_error(void);
void	malloc_error(void);
void	dup_error(void);
void	not_filefd(int *new_fd, int sign, char *after);
char	*get_after(char **found, char **cpy, int *sign, int *new_fd);
int		heredoc_nb(char *cpy, char *found);
void	last_redir(char **after);
int		ft_isnum(char *str);
int		open_fd(char *after, int sign);
void	numdup_badcpy(char **cpy, int *new_fd);
void	numdup_after(char **cpy, int *new_fd, int sign);
void	redir_numdup(char **cpy, int *new_fd, char **after, int sign);
int		invalid_fd(int new_fd, char **after, char **cpy, int sign);
void	get_afterredir(char **after, char **found, char **sign);
char	**tab_insert(char **args, int i, char *ne);
int		redir_init(char *line, int *new_fd, char **cpy);
int		is_redir(char *line);
int		first_redir(char **args);
char	**nb_lastredir(char **args, int i);
void	get_sign(char **found, char *line, char **sign);
char	**after_resize(char **args, int i);
char	**before_resize(char **args, int i);
char	**tab_resize(char **args, int i);
void	get_mode(char *str, int i, int *mode);
int		opin_quote(char *str);
int		check_validity(char **found, char **tmp, int *other, int *j);
int		valid_redir(char *str);
char	*strrchr_quote(char *line, int elem);
int		retab_condition(char **found, char *cpy, char *args);
int		retab_argsdir(char ***args, char **cpy, char **after, int i);
void	nb_retabs(char ***args, int i, char *after);
int		arg_last_redir(char **str, char **cpy2);
void	get_found(char **found, char **after, char *cpy, char **str);
char	**before_pipes(char **args, int i);
char	**retab_dirs(char **args);
void	retab_norm(char **after, char **found);
void	loop_forward(int *i, char **cpy);
void	retab_dirsinit(int *i, int *ret);
void	change_args(char ***args, int i, char **after, char *cpy);
int		get_pipefound(char **found, char **cpy, int i, char **args);
char	**resize_pipes(char **args, int i);
char	**after_pipes(char **args, int i);
void	redir_sleftend(int *fd, int tube[]);
int		get_heredoc(char ***args, int tube[]);
int		apply_heredoc(char ***args, int tube[], int i);
void	do_heredoc(char ***args, int tube[], int i);
char	**redit_init(int tube[], char ***arg, int *i, int *fd);
void	retab_init(char ***args, int *j, char **tmp, int i);
void	inputs_init(void);
void	inputs_reset(void);
void	process_pipe(char ***args);
t_nrm	*apply_init(int tube[], int **o_pid, char ***args, char ***arg);
int		count_pipes(char **args);
void	termios_error(void);
void	flag_init(struct termios *term);
void	del_args(char ***arg);
int		valid_term(void);
void	data_free(void);
void	data_init(void);
int		end_commands(char **str, char **base, char **line);
void	loop_init(char **found, char **str, char **base, char **tmp);

t_inputs	*g_input;
t_data		*g_data;
t_history	*g_history;
t_pipe		*g_pipe;

#endif

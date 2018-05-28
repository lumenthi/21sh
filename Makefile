# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/22 14:06:43 by lumenthi          #+#    #+#              #
#    Updated: 2018/05/28 15:16:12 by lumenthi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

CC = gcc
FLAGS = -Wall -Werror -Wextra
CURSES = -lncurses

LIBDIR = libft
SRCDIR = srcs
INCDIR = includes libft
OBJDIR = objs

LIBFT = $(LIBDIR)/libft.a
LIBFT_OBJ = $(LIBDIR)/objs

SRCS = minishell.c \
		minishell1.c \
		minishell2.c \
		minishell3.c \
		minishell4.c \
		minishell5.c \
		minishell6.c \
		ft_echo.c \
		ft_cd.c \
		ft_env.c \
		ft_env2.c \
		ft_setenv.c \
		ft_unsetenv.c \
		ft_execve.c \
		ft_execve2.c \
		del_tools.c \
		global_tools.c \
		get_args.c \
		get_args2.c \
		error_handling.c \
		ft_error.c \
		ft_error2.c \
		cd.c \
		global_tools2.c \
		termcaps_tools.c \
		signal_handling.c \
		ft_history.c \
		ft_redirections.c \
		ft_redirections1.c \
		ft_redirections2.c \
		ft_redirections3.c \
		ft_redirections4.c \
		ft_redirections5.c \
		ft_redirections6.c \
		ft_redirections7.c \
		ft_redirections8.c \
		ft_redirections9.c \
		ft_quotes.c \
		ft_quotes2.c \
		ft_quotes3.c \
		history.c \
		history2.c \
		line_history.c \
		line_tools.c \
		line_tools2.c \
		line_tools3.c \
		line_tools4.c \
		copy_mode.c \
		copy_mode2.c \
		copy_mode3.c \
		heredoc_mode.c \
		quote_mode.c \
		quote_mode2.c \
		standard_mode.c \
		standard_mode2.c

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS))
INCLUDES = $(addprefix -I, $(INCDIR))
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

vpath %.c srcs \
	srcs/builtins/ft_echo \
	srcs/builtins/ft_cd \
	srcs/builtins/ft_setenv \
	srcs/builtins/ft_unsetenv \
	srcs/builtins/ft_env \
	srcs/builtins/ft_history \
	srcs/tools \
	srcs/args \
	srcs/tools/error \
	srcs/tools/quotes \
	srcs/signals \
	srcs/redirections \
	srcs/history \
	srcs/line_edition \
	srcs/line_edition/copy_mode \
	srcs/line_edition/heredoc_mode \
	srcs/line_edition/quote_mode \
	srcs/line_edition/standard_mode \
	srcs/line_edition/tools \
	srcs/core

GREEN = '\033[4;32m'
RED = '\033[4;31m'
BLANK = '\033[0m'
YELLOW = '\033[4;33m'

TICK = '\033[1;32m~\033[0m'
CROSS = '\033[1;31mx\033[0m'

.PHONY : all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@ $(CC) $(LIBFT) -o $(NAME) $(OBJS) $(CURSES)
	@ printf " %b | Compiled %b%b%b\n" $(TICK) $(GREEN) $(NAME) $(BLANK)

$(OBJDIR)/%.o: %.c
	@ mkdir -p $(OBJDIR)
	@ $(CC) -c $(FLAGS) $(INCLUDES) -o $@ $<

$(LIBFT): $(LIBFT_OBJ)
	@ make -sC $(LIBDIR)

$(LIBFT_OBJ):
	@ make -sC $(LIBDIR)

clean:
	@ make -C $(LIBDIR) clean
	@ test -d $(OBJDIR) && \
	rm -rf $(OBJDIR) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b object folder\n" $(YELLOW) $(NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b object folder\n" $(YELLOW) $(NAME) $(BLANK))

fclean: clean
	@ test -f $(LIBFT) && \
	rm -rf $(LIBFT) && \
	printf " %b | " $(TICK) && \
	printf "Removed %blibft%b library\n" $(RED) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %blibft%b library\n" $(RED) $(BLANK))
	@ test -f $(NAME) && \
	rm -rf $(NAME) && \
	printf " %b | " $(TICK) && \
	printf "Removed %b%b%b executable\n" $(RED) $(NAME) $(BLANK) \
	|| (printf " %b | " $(CROSS) && \
	printf "No %b%b%b executable\n" $(RED) $(NAME) $(BLANK))

re: fclean all

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lumenthi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/22 14:06:43 by lumenthi          #+#    #+#              #
#    Updated: 2018/03/24 11:36:32 by lumenthi         ###   ########.fr        #
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
		ft_echo.c \
		ft_cd.c \
		ft_env.c \
		ft_env2.c \
		ft_setenv.c \
		ft_unsetenv.c \
		ft_execve.c \
		del_tools.c \
		global_tools.c \
		get_args.c \
		error_handling.c \
		ft_error.c \
		cd.c \
		global_tools2.c \
		termcaps_tools.c

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS))
INCLUDES = $(addprefix -I, $(INCDIR))
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

vpath %.c srcs \
	srcs/builtins/ft_echo \
	srcs/builtins/ft_cd \
	srcs/builtins/ft_setenv \
	srcs/builtins/ft_unsetenv \
	srcs/builtins/ft_env \
	srcs/tools \
	srcs/args \
	srcs/tools/error

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

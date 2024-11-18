# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 00:03:28 by fmaurer           #+#    #+#              #
#    Updated: 2024/11/18 18:26:54 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_IN = ./minishell.c \
					./prompt.c \
					./error_exit_utils.c \
					./term_setup.c \
					./signal.c

SRCS = $(patsubst ./%.c,%.c,$(SRCS_IN))

OBJDIR = obj
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

HDR = minishell.h

CC = clang
# CFLAGS = -Wall -Werror -Wextra
CFLAGS =

# adding libft
LIBFT_PATH	= ./libft
LIBFT				= $(LIBFT_PATH)/libft.a
LIB_PATHS += -L$(LIBFT_PATH)
LIBS += -lft -lreadline

GRN = \033[1;32m
RED = \033[1;31m
WHT = \033[1;37m
EOC = \033[1;0m
YLW = \033[1;33m
MSGOPN = $(YLW)[[$(GRN)
MSGEND = $(YLW)]]$(EOC)

log_msg = $(MSGOPN) $(1) $(MSGEND)

all: $(NAME)

$(OBJDIR)/%.o: %.c $(HDR)| $(OBJDIR)
	@echo -e "$(call log_msg,Compiling $<...)"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) | $(LIBFT)
	@echo -e "$(call log_msg,Compiling $(NAME)...)"
	$(CC) $(CFLAGS) $(LIB_PATHS) -o $@ $^ $(LIBS)

$(LIBFT):
	@echo -e "$(call log_msg,Compiling libft...)"
	make -C $(LIBFT_PATH) all

debug: $(SRCS)
	$(CC) -g $(CFLAGS) -o pipex $^

$(OBJDIR):
	mkdir -p obj

clean:
	@echo -e "$(call log_msg,Removing libft objs.)"
	@make -s -C $(LIBFT_PATH) clean
	@echo -e "$(call log_msg,Removing minishell objs.)"
	@rm -rf $(OBJDIR)

fclean: clean
	@echo -e "$(call log_msg,fcleaning libft..)"
	@make -s -C $(LIBFT_PATH) fclean
	@echo -e "$(call log_msg,fcleaning minishell..)"
	@rm -f $(NAME)

.PHONY: all clean fclean debug

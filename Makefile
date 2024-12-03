# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 00:03:28 by fmaurer           #+#    #+#              #
#    Updated: 2024/12/03 16:08:36 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS_IN = ./minishell.c \
					./term_setup.c \
					./signal.c \
					./utils.c \
					./error_exit_utils.c \
					./tok_tokenize.c \
					./tok_cmdline.c \
					./tok_tokenlist.c \
					./tok_tokprint.c \
					./tok_lvl1_tokenize.c \
					./tok_lvl1_get_next_tok.c \
					./tok_lvl1_get_next_helpers1.c \
					./tok_lvl1_get_next_helpers2.c \
					./tok_lvl2_tokenize.c \
					./tok_lvl2_check_toklst.c \
					./par_cmdlist.c \
					./par_parsing.c \
					./par_parsing_helpers1.c \
					./env.c \
					./env_envlst.c \
					./env_parse.c \
					./bltin_export.c \
					./exec.c \
					./exec_get_exec_path.c

SRCS = $(patsubst ./%.c,%.c,$(SRCS_IN))

OBJDIR = obj
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

HDR = minishell.h

RL_PATH = ./readline
RL_LIBS = $(RL_PATH)/libreadline.a $(RL_PATH)/libhistory.a

CC = clang
# CFLAGS = -Wall -Werror -Wextra
CFLAGS =

# adding libft
LIBFT_PATH	= ./libft
LIBFT				= $(LIBFT_PATH)/libft.a
LIB_PATHS += -L$(LIBFT_PATH) -L$(RL_PATH)
## Libs for prod:
# LIBS += -lft -lreadline
## temporary libs for dev:
LIBS_DEV += -lft -lncurses

GRN = \033[1;32m
RED = \033[1;31m
WHT = \033[1;37m
EOC = \033[1;0m
YLW = \033[1;33m
MSGOPN = $(YLW)[[$(GRN)
MSGEND = $(YLW)]]$(EOC)

log_msg = $(MSGOPN) $(1) $(MSGEND)

# all: $(NAME)
all: debug

$(OBJDIR)/%.o: %.c $(HDR)| $(OBJDIR)
	@echo -e "$(call log_msg,Compiling $<...)"
	$(CC) $(CFLAGS) -c $< -o $@

## This is the recipe for prod, without readline submodule
# $(NAME): $(OBJS) | $(LIBFT)
# 	@echo -e "$(call log_msg,Compiling $(NAME)...)"
# 	$(CC) $(CFLAGS) $(LIB_PATHS) -o $@ $^ $(LIBS) -lreadline

$(NAME): $(OBJS) | $(LIBFT)
	@echo -e "$(call log_msg,Compiling $(NAME)...)"
	$(CC) $(CFLAGS) $(LIB_PATHS) -o $@ $^ $(LIBS_DEV) $(RL_LIBS)

$(LIBFT):
	@echo -e "$(call log_msg,Compiling libft...)"
	make -C $(LIBFT_PATH) all

# readline submodule
$(RL_LIBS):
	@echo -e "$(call log_msg,Compiling readline...)"
	cd ./readline && ./configure
	make -C ./readline

debug: $(SRCS) $(RL_LIBS)| $(LIBFT)
	@echo -e "$(call log_msg,Compiling debug...)"
	$(CC) $(CFLAGS) -g -DDEBUG -DREADLINE_LIBRARY $(LIB_PATHS) -o $(NAME) $^ $(LIBS_DEV) $(RL_LIBS)

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

# re: fclean $(NAME)
re: fclean all

.PHONY: all clean fclean debug re

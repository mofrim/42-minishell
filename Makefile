# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elpah <elpah@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 00:03:28 by fmaurer           #+#    #+#              #
#    Updated: 2025/01/04 04:35:14 by elpah            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR		=	./src

# using the *magic* VPATH variable for finding my sources 🤯. also possible to
# use it like: `vpath %.c dir1/ dir2/ ...`. more on this ->
# https://makefiletutorial.com/#the-vpath-directive
#
# TLDR; this will make make find the correct source file `./src/exec/bla.c` for
# any obj called `obj/bla.o`
VPATH	=	./src ./src/exec ./src/parsing ./src/env ./src/builtins \
				./src/tokenization

SRCS =	minishell.c \
				term_setup.c \
				signal.c \
				utils.c \
				error_exit_utils.c \
				tok_tokenize.c \
				tok_cmdline.c \
				tok_tokenlist.c \
				tok_tokenlist_del.c \
				tok_tokprint.c \
				tok_utils.c \
				tok_tokerr_int.c \
				tok_lvl1_tokenize.c \
				tok_lvl1_get_next_tok.c \
				tok_lvl1_get_next_helpers1.c \
				tok_lvl1_get_next_helpers2.c \
				tok_lvl1_get_next_var.c \
				tok_lvl1_get_next_redir_out.c \
				tok_lvl1_get_next_redir_in.c \
				tok_lvl2_tokenize.c \
				tok_lvl2_check_toklst.c \
				tok_lvl2_redir_tok.c \
				tok_lvl3_tokenize.c \
				par_cmdlist.c \
				par_heroflst.c \
				par_parsing.c \
				par_parse_redir_out.c \
				par_parse_redir_in.c \
				par_redirlst.c \
				par_parse_pipe.c \
				par_parse_cmd_bltin.c \
				par_parse_heredoc.c \
				env_utils.c \
				env_envlst.c \
				env_parse.c \
				env_get_env_array.c \
				env_set_entry.c \
				env_copy.c \
				env_export_utils.c \
				bltin_export.c \
				bltin_cd.c \
				bltin_echo.c \
				bltin_env.c \
				bltin_exit.c \
				bltin_pwd.c \
				bltin_unset.c \
				exec.c \
				exec_utils.c \
				exec_get_exec_path.c \
				exec_single.c \
				exec_single_builtin.c \
				exec_open_redir_files.c \
				exec_proc_redirlst_in.c \
				exec_proc_redirlst_out.c \
				exec_pipeline.c \
				exec_pipe_cmd.c \
				exec_pipe_bltin.c

OBJDIR = obj
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
OBJS_DBG = $(patsubst %.c,$(OBJDIR)/dbg-%.o,$(SRCS))

RL_PATH = ./readline
RL_LIBS = $(RL_PATH)/libreadline.a $(RL_PATH)/libhistory.a

CC = clang
CFLAGS = -Wall -Werror -Wextra

# adding libft
LIBFT_PATH	= ./libft
LIBFT				= $(LIBFT_PATH)/libft.a
LIB_PATHS += -L$(LIBFT_PATH) -L$(RL_PATH)

INC_DIR	= $(SRC_DIR)/include
INC	= -I$(INC_DIR) -I$(LIBFT_PATH) -I$(RL_PATH)
HDRS = $(INC_DIR)/minishell.h $(INC_DIR)/colors.h

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

all: $(NAME)
# all: debug
# all:
# 	echo $(SRCS)

$(OBJDIR)/%.o: %.c $(HDRS)| $(OBJDIR)
	@echo -e "$(call log_msg,Compiling $<...)"
	$(CC) $(CFLAGS) $(INC) -DREADLINE_LIBRARY -c $< -o $@

$(OBJDIR)/dbg-%.o: %.c $(HDRS)| $(OBJDIR)
	@echo -e "$(call log_msg,Compiling $<...)"
	$(CC) $(CFLAGS) $(INC) -g -O0 -DDEBUG -DREADLINE_LIBRARY -c $< -o $@

## This is the recipe for prod, without readline submodule
# $(NAME): $(OBJS) | $(LIBFT)
# 	@echo -e "$(call log_msg,Compiling $(NAME)...)"
# 	$(CC) $(CFLAGS) $(LIB_PATHS) $(INC) -o $@ $^ $(LIBS) -lreadline

$(NAME): $(OBJS) | $(LIBFT) $(RL_LIBS)
	@echo -e "$(call log_msg,Compiling $(NAME)...)"
	$(CC) $(CFLAGS) -DREADLINE_LIBRARY $(LIB_PATHS) $(INC) -o $@ $^ $(LIBS_DEV) \
		$(RL_LIBS)

$(LIBFT):
	@echo -e "$(call log_msg,Compiling libft...)"
	make -C $(LIBFT_PATH) all

# readline submodule
$(RL_LIBS):
	@echo -e "$(call log_msg,Compiling readline...)"
	cd ./readline && ./configure
	make -C ./readline

# FIXME
# empty rule ?!?! i just want change detection for them in the end...
$(HDRS):

debug: $(OBJS_DBG) | $(LIBFT) $(RL_LIBS)
	@echo -e "$(call log_msg,Compiling debug...)"
	$(CC) $(CFLAGS) -g -O0 -DDEBUG -DREADLINE_LIBRARY $(LIB_PATHS) $(INC) -o \
		$(NAME) $^ $(LIBS_DEV) $(RL_LIBS)

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

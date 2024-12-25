# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elpah <elpah@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/02 00:03:28 by fmaurer           #+#    #+#              #
#    Updated: 2024/12/25 17:37:57 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_DIR		= ./src
EXEC_DIR	= $(SRC_DIR)/exec
PARSE_DIR	= $(SRC_DIR)/parsing
ENV_DIR	= $(SRC_DIR)/env
BLTIN_DIR	= $(SRC_DIR)/builtins
TOK_DIR	= $(SRC_DIR)/tokenization


SRCS =	$(SRC_DIR)/minishell.c \
				$(SRC_DIR)/term_setup.c \
				$(SRC_DIR)/signal.c \
				$(SRC_DIR)/utils.c \
				$(SRC_DIR)/error_exit_utils.c \
				$(TOK_DIR)/tok_tokenize.c \
				$(TOK_DIR)/tok_cmdline.c \
				$(TOK_DIR)/tok_tokenlist.c \
				$(TOK_DIR)/tok_tokenlist_del.c \
				$(TOK_DIR)/tok_tokprint.c \
				$(TOK_DIR)/tok_utils.c \
				$(TOK_DIR)/tok_tokerr_int.c \
				$(TOK_DIR)/tok_lvl1_tokenize.c \
				$(TOK_DIR)/tok_lvl1_get_next_tok.c \
				$(TOK_DIR)/tok_lvl1_get_next_helpers1.c \
				$(TOK_DIR)/tok_lvl1_get_next_helpers2.c \
				$(TOK_DIR)/tok_lvl1_get_next_var.c \
				$(TOK_DIR)/tok_lvl1_get_next_redir_out.c \
				$(TOK_DIR)/tok_lvl2_tokenize.c \
				$(TOK_DIR)/tok_lvl2_check_toklst.c \
				$(TOK_DIR)/tok_lvl2_redir_tok.c \
				$(TOK_DIR)/tok_lvl3_tokenize.c \
				$(PARSE_DIR)/par_cmdlist.c \
				$(PARSE_DIR)/par_heroflst.c \
				$(PARSE_DIR)/par_parsing.c \
				$(PARSE_DIR)/par_parse_redir.c \
				$(PARSE_DIR)/par_redirlst.c \
				$(PARSE_DIR)/par_parsing_helpers1.c \
				$(PARSE_DIR)/par_parse_cmd_bltin.c \
				$(PARSE_DIR)/par_parse_heredoc.c \
				$(ENV_DIR)/env_utils.c \
				$(ENV_DIR)/env_envlst.c \
				$(ENV_DIR)/env_parse.c \
				$(ENV_DIR)/env_get_env_array.c \
				$(ENV_DIR)/env_set_entry.c \
				$(ENV_DIR)/env_copy.c \
				$(BLTIN_DIR)/bltin_export.c \
				$(BLTIN_DIR)/bltin_cd.c \
				$(BLTIN_DIR)/bltin_echo.c \
				$(BLTIN_DIR)/bltin_env.c \
				$(BLTIN_DIR)/bltin_exit.c \
				$(BLTIN_DIR)/bltin_pwd.c \
				$(BLTIN_DIR)/bltin_unset.c \
				$(EXEC_DIR)/exec.c \
				$(EXEC_DIR)/exec_get_exec_path.c \
				$(EXEC_DIR)/exec_single.c \
				$(EXEC_DIR)/exec_single_redir.c \
				$(EXEC_DIR)/exec_single_builtin.c \
				$(EXEC_DIR)/exec_open_redir_files.c \
				$(EXEC_DIR)/exec_proc_redirlst_helpers.c \
				$(EXEC_DIR)/exec_pipe.c


# SRCS = $(patsubst ./%.c,%.c,$(SRCS_IN))

OBJDIR = obj
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

RL_PATH = ./readline
RL_LIBS = $(RL_PATH)/libreadline.a $(RL_PATH)/libhistory.a

CC = clang
# CFLAGS = -Wall -Werror -Wextra
CFLAGS =

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

# all: $(NAME)
all: debug

$(OBJDIR)/%.o: %.c $(HDRS)| $(OBJDIR)
	@echo -e "$(call log_msg,Compiling $<...)"
	$(CC) $(CFLAGS) -c $< -o $@

## This is the recipe for prod, without readline submodule
# $(NAME): $(OBJS) | $(LIBFT)
# 	@echo -e "$(call log_msg,Compiling $(NAME)...)"
# 	$(CC) $(CFLAGS) $(LIB_PATHS) -o $@ $^ $(LIBS) -lreadline

$(NAME): $(OBJS) | $(LIBFT)
	@echo -e "$(call log_msg,Compiling $(NAME)...)"
	$(CC) $(CFLAGS) $(LIB_PATHS) $(INC) -o $@ $^ $(LIBS_DEV) $(RL_LIBS)

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

debug: $(SRCS) $(RL_LIBS) $(HDRS)| $(LIBFT)
	@echo -e "$(call log_msg,Compiling debug...)"
	$(CC) $(CFLAGS) -g -O0 -DDEBUG -DREADLINE_LIBRARY $(LIB_PATHS) $(INC) -o $(NAME) $(SRCS) $(LIBS_DEV) $(RL_LIBS)

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

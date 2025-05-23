CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

SRCS = \
    main/shell.c \
    parsing/tokenize.c \
    parsing/parser.c \
    parsing/lexer.c \
    builtins/echo.c \
    builtins/cd.c \
    builtins/env.c \
    builtins/pwd.c \
    builtins/unset.c \
    helper_funcs/helpers1.c \
    helper_funcs/allocator.c \
    helper_funcs/helpers2.c \
    helper_funcs/helpers3.c \
    execution/executor.c \
    execution/execute_one_cmd.c \
    execution/execute_multiple_cmd \
    execution/files_checks.c \
    execution/get_path.c \
    execution/heredoc_1.c \
    execution/heredoc_2.c \
    expansions/var_tools.c \
    expansions/handle_exp.c \
    expansions/expand_vars.c \
    expansions/expand_exit_status.c

OBJS = $(SRCS:.c=.o)

INCLUDES = minishell.h

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)

LIBS = -lreadline

all: $(LIBFT_LIB) $(NAME)

$(LIBFT_LIB): 
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT_LIB) $(LIBS) $(LIBFT_INC)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

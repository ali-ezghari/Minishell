CC = cc
CFLAGS = -Wall -Wextra -Werror -g

NAME = minishell

SRCS = \
    main/shell.c \
    parsing/tokenize.c \
    parsing/parser.c \
    parsing/lexer.c \
    parsing/syntax_check.c \
    parsing/syntax_utils.c \
    helper_funcs/helpers1.c \
    helper_funcs/helpers2.c \
    helper_funcs/helpers3.c \
    helper_funcs/helpers4.c \
    helper_funcs/allocator.c \
    builtins/echo.c \
    builtins/exit.c \
    builtins/unset.c \
    builtins/cd.c \
    builtins/env.c \
    builtins/pwd.c \
    builtins/export.c \
    builtins/export2.c \
    execution/executor.c \
    execution/execute_one_cmd.c \
    execution/execute_multiple_cmd.c \
    execution/files_checks.c \
    execution/get_path.c \
    execution/heredoc_1.c \
    execution/heredoc_2.c \
    expansions/var_tools.c \
    expansions/handle_exp.c \
    expansions/expand_vars.c \
    expansions/expand_exit_status.c \
    execution/environment.c \
    libft/lib1.c \
    libft/lib2.c \
    libft/lib3.c \
    libft/lib4.c \
    libft/lib5.c \
    libft/lib6.c 
OBJS = $(SRCS:.c=.o)

INCLUDES = minishell.h


LIBS = -lreadline

all:$(NAME)


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

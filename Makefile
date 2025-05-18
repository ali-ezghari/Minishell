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
    helper_funcs/allocator.c
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

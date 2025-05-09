#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

/**
 * Linked list node representing an environment variable.
 * Stores the variable's key and value.
 */
typedef struct s_env_node
{
    char *key;
    char *value;

    struct s_env_node *next;
} t_env_node;

typedef enum s_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
} t_token_type;

/**
 * Represents a single token from the input.
 * - type: The token's type (e.g., TOKEN_REDIR_OUT for ">")
 * - value: The string value of the token
 */
typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;


char	**input_split(char *input);

#endif

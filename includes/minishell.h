#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include "../libft/libft.h"
#include "utils.h"

typedef enum e_token_type
{
    T_WORD,         // general word (e.g., command or argument)
    T_PIPE,         // |
    T_REDIRECT_IN,  // <
    T_REDIRECT_OUT, // >
    T_APPEND,       // >>
    T_HEREDOC       // <<
} t_token_type;

typedef struct s_env
{
    char *key;   // e.g., "PATH"
    char *value; // e.g., "/usr/bin:/bin"
    struct s_env *next;
} t_env;

typedef struct s_token
{
    char *value;          // str content
    t_token_type type;    // type of the token
    struct s_token *next; // next token
} t_token;

/*
** Linked list of redirections attached to a command
*/
typedef struct s_redir
{
    t_token_type type;    // >, >>, <, <<
    char *filename;       // target file or limiter str
    struct s_redir *next; // next redir in the list
} t_redir;

/*
** Represents a single command in the pipeline, e.g.:
** echo hi | grep h > out.txt
** Each command is one node in the pipeline.
*/
typedef struct s_command
{
    char **av;              // av (NULL-terminated)
    t_redir *redirs;        // list of redir (if any)
    struct s_command *next; // next command in a pipeline
    int is_builtin; // set to 1 if its builtin, 0 otherwise
} t_command;

/*
** shell context: shared between parser and executor
*/
typedef struct s_shell
{
    t_env *envp;
    t_token *tokens; // token list from lexer
    t_command *cmds; // command list from parser
    int exit_status; // last exit code
    t_allocator *allocator; // the memory list
} t_shell;

//
// TOKENIZER / LEXER FUNCTIONS
//

char **tokenize(const char *input);
char *strndup_custom(const char *s, size_t n);
void skip_spaces(const char **str);
char *get_quoted_token(const char **str);
char *get_operator_token(const char **str);
char *get_env_var_token(const char **str);
char *get_word_token(const char **str);
t_token *build_lexed_tokens(char **token_array);


//
// builtins
//

void bin_pwd(t_shell *shell);
void bin_cd(t_command *cmd, t_shell *shell);
void bin_echo(t_command *cmd, t_shell *shell);
void bin_env(t_command *cmd, t_shell *shell);
#endif

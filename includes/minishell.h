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
    int fd;
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
} t_command;

typedef struct s_shell
{
    t_env *envp;
    t_allocator *gc;
    t_token *tokens;
    t_command *cmds;
    int is_forked;
    int exit_status;

    int in_fd_b;
    int out_fd_b;
} t_shell;

//
// TOKENIZER / LEXER FUNCTIONS
//

char **tokenize(const char *input, t_allocator **gc);
char *strndup_custom(const char *s, size_t n);
void skip_spaces(const char **str);
char *get_quoted_token(const char **str);
char *get_operator_token(const char **str, t_allocator **gc);
char *get_env_var_token(const char **str);
char *get_word_token(const char **str);
t_token *build_lexed_tokens(char **token_array, t_allocator **gc);

//
// PARSING
//

t_command *parse_tokens(t_token *tokens, t_allocator **gc);


//
// builtins
//

void bin_pwd(t_shell *shell);
void bin_cd(t_command *cmd, t_shell *shell);
void bin_echo(t_command *cmd, t_shell *shell);
void bin_env(t_command *cmd, t_shell *shell);
void bin_unset(t_command *cmd, t_shell *shell);
void bin_exit(t_command *cmd, t_shell *shell);
void bin_export(t_command *cmd, t_shell *shell);

// EXECTION
int open_files(t_redir *redir, t_shell *shell);
int execute_builtin(t_command *cmd, t_shell *shell);
void execute_one_cmd(t_command *cmd, t_shell *shell);
void execution(t_shell *shell);
//
// HEREDOC
//
int handle_heredoc(t_command *cmd, t_shell *shell);
char *quote_remover(char *del, int *expand, t_shell *shell);

//
// Helpers
//
void close_files(t_redir *redir);
void in_out_backup(t_shell *shell);
int ft_strcmp(const char *s1, const char *s2);
void free_token_list(t_token *head);
void free_token_array(char **arr);
char *ft_getenv(char *key, t_shell *shell);
void pipe_err(t_shell *shell);
void dup_in(int fd, t_shell *shell);
void dup_out(int fd, t_shell *shell);
void restore_fds(int out, int in);
void custom_cmd_err(char *msg, char *name, int status, t_shell *shell);
char *ft_strjoin_three(char *s1, char *s2, char *s3);
void add_string_array_to_allocator(char **arr, t_allocator **gc);
#endif

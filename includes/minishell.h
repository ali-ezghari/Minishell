/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:32:26 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/26 18:14:33 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TRUE 1
# define FALSE 0

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ctype.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include "utils.h"
#include <sys/stat.h>
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIRECT_IN,
	T_REDIRECT_OUT,
	T_APPEND,
	T_HEREDOC
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
	int				fd;
}					t_redir;

typedef struct s_command
{
	char				**av;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	t_env		*envp;
	char		**envs;
	t_allocator	*gc;
	t_token		*tokens;
	t_command	*cmds;
	int			is_forked;
	int			exit_status;
	int			in_fd_b;
	int			out_fd_b;
}				t_shell;

//
// TOKENIZER / LEXER FUNCTIONS
//

char	**tokenize(const char *inp, t_allocator **gc);
char	*ft_strndup(const char *s, size_t n, t_allocator **gc);
void	skip_spaces(const char **str);
char	*get_quoted_token(const char **str, t_allocator **gc);
char	*get_operator_token(const char **str, t_allocator **gc);
char	*get_env_var_token(const char **str, t_allocator **gc);
char	*get_word_token(const char **str, t_allocator **gc);
t_token	*build_lexed_tokens(char **token_array, t_allocator **gc);
t_token	*new_token(char *val, t_allocator **gc);
t_token_type	get_token_type(const char *s);

//
// PARSING
//
t_command	*parse_tokens(t_token *tokens, t_shell *shell);
int			catch_syntax_errors(t_token *tok, t_shell *shell);
//
// builtins
//

void	bin_pwd(t_shell *shell);
void	bin_cd(t_command *cmd, t_shell *shell);
void	bin_echo(t_command *cmd, t_shell *shell);
void	bin_env(t_command *cmd, t_shell *shell);
void	bin_unset(t_command *cmd, t_shell *shell);
void	bin_exit(t_command *cmd, t_shell *shell);
void	bin_export(t_command *cmd, t_shell *shell);

// EXECTION
int		open_files(t_redir *redir, t_shell *shell);
int		execute_builtin(t_command *cmd, t_shell *shell);
void	execute_one_cmd(t_command *cmd, t_shell *shell);
void	execution(t_shell *shell);
char	*get_path1(char *cmd, t_shell *shell);
void	close_files(t_redir *redir);
char	*ft_getenv(char *key, t_env *env);
void	get_exit_code(int status, t_shell *shell);
void	execute_multiple_cmds(int count, t_command *cmd, t_shell *shell);
void	clean_up_fds(int count, int *fds);
int		init_pids_fds(int cmd_count, pid_t **pids, int **fds, t_shell *shell);
//
// HEREDOC
//
int		handle_heredoc(t_command *cmd, t_shell *shell);
char	*quote_remover(char *del, int *expand, t_shell *shell);

//
// Helpers
//
void	add_env_back(t_env **env_list, t_env *new_env);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin_three(char *s1, char *s2, char *s3);
void	in_out_backup(t_shell *shell);
void	free_token_list(t_token *head);
void	free_token_array(char **arr);
void	pipe_err(t_shell *shell);
void	dup_in(int fd);
void	dup_out(int fd);
void	restore_fds(int in, int out);
void	custom_err(char *msg, char *name, int status, t_shell *shell);
void	add_string_array_to_allocator(char **arr, t_allocator **gc);
void	sort_env_list(t_env *env);
t_env	*new_env(char *key, char *value, t_shell *shell);

//
// Variable Expansion Functions
//
int		is_var_name(char c);
int		is_valid_var_name(char *name);
char	*handle_exp(char *inp, t_shell *shell, int exit_status);
char	*ft_expand_vars(char *str, t_shell *shell);
char	*ft_expand_exit_status(char *str, int exit_status, t_allocator **gc);
void	allocation_failure(t_shell *shell);
t_env	*env_new(char *name, char *value, t_allocator **gc);
char	*get_identifier(char *str, t_shell *shell);
int		check_identifier(char *cmd, t_shell *shell);
void	print_sorted_env(t_env *env, t_shell *shell);
int		print_synerror(const char *content);
int		check_unclosed_quotes(const char *line);
#endif

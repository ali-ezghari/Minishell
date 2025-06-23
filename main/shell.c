#include "../includes/minishell.h"
#include "../includes/utils.h"

void print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("    Redir: type=%d, target=%s\n", redir->type, redir->filename);
		redir = redir->next;
	}
}

void print_commands(t_command *cmd)
{
    int i = 0;
    while (cmd)
    {
        printf("Command %d:\n", i++);
        int j = 0;
        if (cmd->av)
        {
            while (cmd->av[j])
            {
                printf("  Arg[%d]: %s\n", j, cmd->av[j]);
                j++;
            }
        }
        print_redirs(cmd->redirs);
        cmd = cmd->next;
    }
}

void print_tokens(t_token *tokens)
{
	int i = 0;
	printf("=== TOKENS ===\n");
	while (tokens)
	{
		printf("Token[%d]: '%s' (type: %d)\n", i++, tokens->value, tokens->type);
		tokens = tokens->next;
	}
	printf("==============\n");
}

void free_token_list(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}

void free_token_array(char **arr)
{
	int i = 0;
	if (!arr)
		return;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void free_redirs(t_redir *redir)
{
	t_redir *tmp;
	while (redir)
	{
		tmp = redir->next;
		free(redir->filename);
		free(redir);
		redir = tmp;
	}
}

void free_commands(t_command *cmd)
{
	t_command *tmp;
	while (cmd)
	{
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
}

t_env *init_env(char **envp, t_allocator **gc)
{
	t_env *head = NULL;
	t_env *current = NULL;
	t_env *new_node;
	char *equal_sign;
	int key_len;

	while (*envp)
	{
		new_node = ft_malloc(sizeof(t_env), gc);
		if (!new_node)
			return NULL;
		equal_sign = ft_strchr(*envp, '=');
		if (!equal_sign)
		{
			envp++;
			continue;
		}
		key_len = equal_sign - *envp;
		new_node->key = ft_substr(*envp, 0, key_len);
		new_node->value = ft_strdup(equal_sign + 1);
		new_node->next = NULL;
		add_to_allocator(new_node->key, gc);
		add_to_allocator(new_node->value, gc);
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		envp++;
	}
	return head;
}

void cleanup_shell(t_shell *shell)
{
    if (shell->tokens)
        free_token_list(shell->tokens);
    if (shell->cmds)
        free_commands(shell->cmds);
    shell->tokens = NULL;
    shell->cmds = NULL;
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *line;
	char **token_array;

	(void)argc;
	(void)argv;
	shell.gc = NULL;
	shell.envs = envp;
	shell.envp = init_env(envp, &shell.gc);
	shell.exit_status = 0;
	shell.tokens = NULL;
	shell.cmds = NULL;
	printf("Enter commands to test parsing. Type 'exit' to quit.\n\n");
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			break;
		}
		if (*line)
			add_history(line);
		if (!*line || (*line == ' ' && !*(line + 1)))
		{
			free(line);
			continue;
		}
		if (check_unclosed_quotes(line))
		{
			printf("Quote check failed!\n");
			free(line);
			continue;
		}
		printf("Quote check passed\n");
		token_array = tokenize(line, &shell.gc, &shell);
		if (!token_array)
		{
			printf("Tokenization failed\n");
			free(line);
			continue;
		}
		printf("Tokenization completed\n");
		shell.tokens = build_lexed_tokens(token_array, &shell.gc);
		if (!shell.tokens)
		{
			printf("Lexer failed\n");
			free(line);
			continue;
		}
		printf("Lexer completed\n");
		print_tokens(shell.tokens);
		shell.cmds = parse_tokens(shell.tokens, &shell);
		if (!shell.cmds)
		{
			printf("Parsing failed (syntax error or other issue)\n");
			if (shell.exit_status == 2)
				printf("Exit status: %d (syntax error)\n", shell.exit_status);
		}
		else
		{
			printf("Parsing completed successfully\n");
			print_commands(shell.cmds);
			if (shell.cmds->av && shell.cmds->av[0] && 
				strcmp(shell.cmds->av[0], "exit") == 0)
			{
				execution(&shell);
				break;
			}
			else
			{
				execution(&shell);
			}
		}
		cleanup_shell(&shell);
		free(line);
	}
	return 0;
}

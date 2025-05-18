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
		for (int j = 0; cmd->av && cmd->av[j]; j++)
			printf("  Arg[%d]: %s\n", j, cmd->av[j]);
		print_redirs(cmd->redirs);
		cmd = cmd->next;
	}
}

void free_token_list(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}

void free_token_array(char **arr)
{
	int i = 0;
	if (!arr) return;
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
		for (int i = 0; cmd->av && cmd->av[i]; i++)
			free(cmd->av[i]);
		free(cmd->av);
		free_redirs(cmd->redirs);
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

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char *line;
	char **token_array;

	(void)argc;
	(void)argv;
	shell.gc = NULL;
	shell.envp = init_env(envp, &shell.gc);
	shell.exit_status = 0;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (*line)
			add_history(line);

        token_array = tokenize(line, &shell.gc);
        shell.tokens = build_lexed_tokens(token_array, &shell.gc);
        shell.cmds = parse_tokens(shell.tokens, &shell.gc);

		print_commands(shell.cmds);

		free_token_array(token_array);
		free_token_list(shell.tokens);
		free_commands(shell.cmds);
		free(line);
	}
	free_all(&shell.gc);
	return 0;
}

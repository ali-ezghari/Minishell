#include "../includes/minishell.h"

t_env	*env_new(char *name, char *value, t_allocator **gc)
{
	t_env *new = ft_malloc(sizeof(t_env), gc);
	if (!new)
		return (NULL);
	new->key = ft_strdup(name);
	if (!new->key)
	{
		free(new);
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new->key);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	env_add_back(t_env **envp, t_env *new)
{
	t_env *last = *envp;
	if (!last)
	{
		*envp = new;
		return;
	}
	while (last->next)
		last = last->next;
	last->next = new;
}

int	main(void)
{
	t_shell shell;
	t_env *envp = NULL;
	char *input;
	char *result;
	t_env *new_env;
	t_allocator *gc = NULL;
	
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.gc = gc;
	new_env = env_new("USER", "fatouil", &shell.gc);
	if (!new_env)
	{
		printf("Error: Failed to create USER environment variable\n");
		free_all(&shell.gc);
		return (1);
	}
	env_add_back(&envp, new_env);
	new_env = env_new("HOME", "/home/fatouil", &shell.gc);
	if (!new_env)
	{
		printf("Error: Failed to create HOME environment variable\n");
		free_all(&shell.gc);
		return (1);
	}
	env_add_back(&envp, new_env);

	new_env = env_new("SHELL", "/bin/bash", &shell.gc);
	if (!new_env)
	{
		printf("Error: Failed to create SHELL environment variable\n");
		free_all(&shell.gc);
		return (1);
	}
	env_add_back(&envp, new_env);
	shell.envp = envp;
	input = ft_malloc(ft_strlen("Hello $USER, your home is \"$HOME\" and last status was $?") + 1, &shell.gc);
	if (!input)
	{
		printf("Error: Failed to allocate input string\n");
		free_all(&shell.gc);
		return (1);
	}
	strcpy(input, "Hello $USER, your home is \"$HOME\" and last status was $?");
	result = handle_exp(input, &shell, 42);
	if (!result)
	{
		printf("Error: Variable expansion failed\n");
		free_all(&shell.gc);
		return (1);
	}
	printf("Expanded: %s\n", result);
	free_all(&shell.gc);
	return (0);
}

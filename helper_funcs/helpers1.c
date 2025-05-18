#include "../includes/minishell.h"

void in_out_backup(t_shell *shell)
{
    shell->in_fd_b = dup(STDIN_FILENO);
    shell->out_fd_b = dup(STDOUT_FILENO);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *ft_getenv(char *key, t_shell *shell)
{
    t_env *env;
    char *tmp;

    tmp = key;
    env = shell->envp;
    if (!key || !env)
        return (NULL);
    if (!tmp)
        return (NULL);
    if (*tmp == '$')
        tmp++;
    if (*tmp == '\0')
        return (NULL);
    while (env)
    {
        if (!ft_strcmp(tmp, env->key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}
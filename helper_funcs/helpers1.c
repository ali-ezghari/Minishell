#include "../includes/minishell.h"

void custom_err(char *msg, char *name, int status, t_shell *shell)
{
    write(2, "minishell: ", 11);
    if (name)
    {
        write(2, name, strlen(name));
        write(2, ": ", 2);
    }
    if (msg)
        write(2, msg, strlen(msg));
    write(2, "\n", 1);
    shell->exit_status = status;
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char *ft_getenv(char *key, t_env *env)
{
    if (!key || !env)
        return (NULL);
    if (!key)
        return (NULL);
    if (*key == '$')
        key++;
    if (*key == '\0')
        return (NULL);
    while (env)
    {
        if (!ft_strcmp(key, env->key))
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

void pipe_err(t_shell *shell)
{
    perror("Pipe error");
    shell->exit_status = 1;
}
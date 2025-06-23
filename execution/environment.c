#include "../includes/minishell.h"

static int env_list_size(t_env *env)
{
    int count;

    count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return (count);
}

static char *join_env_var(char *key, char *value, t_shell *shell)
{
    int len;
    char *str;

    len = ft_strlen(key) + ft_strlen(value) + 2;
    str = ft_malloc(len, &shell->gc);
    if (!str)
        return (allocation_failure(shell), NULL);
    ft_strlcpy(str, key, len);
    ft_strlcat(str, "=", len);
    ft_strlcat(str, value, len);
    return (str);
}

char **env_list_to_array(t_env *env, t_shell *shell)
{
    int size;
    char **env_array;
    int i;
    
    i = 0;
    size = env_list_size(env);
    env_array = malloc((size + 1) * sizeof(char *));
    if (!env_array)
        return (allocation_failure(shell), NULL);
    while (env)
    {
        env_array[i] = join_env_var(env->key, env->value, shell);
        if (!env_array[i])
            return (allocation_failure(shell), NULL);
        i++;
        env = env->next;
    }
    env_array[i] = NULL;
    return env_array;
}
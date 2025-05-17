#include "../includes/minishell.h"

static t_env *free_var(t_shell *shell, t_env *curr, t_env *prev)
{
    t_env *to_free;

    to_free = curr;
    if (prev)
        prev->next = curr->next;
    else
        shell->envp = curr->next;
    curr = curr->next;
    free(to_free->key);
    free(to_free->value);
    free(to_free);
    return (curr);
}

static void unset_one_var(char *var, t_shell *shell)
{
    t_env *prev;
    t_env *curr;

    prev = NULL;
    curr = shell->envp;
    while (curr)
    {
        if (!ft_strcmp(var, curr->key))
            curr = free_var(shell, curr, prev);
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void bin_unset(t_command *cmd, t_shell *shell)
{
    int i;

    i = 1;
    while (cmd->av[i])
    {
        unset_one_var(cmd->av[i], shell);
        i++;
    }
    shell->exit_status = 0;
} 
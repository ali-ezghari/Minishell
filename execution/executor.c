#include "../includes/minishell.h"

void execute_one_cmd(t_shell *shell)
{
    if (!strncmp(shell->cmds->av[0], "echo", 5))
        bin_echo(shell);
    else if (!strncmp(shell->cmds->av[0], "cd", 3))
        bin_cd(shell);
    else if (!strncmp(shell->cmds->av[0], "export", 7))
        bin_export(shell);
    else if (!strncmp(shell->cmds->av[0], "unset", 7))
        bin_unset(shell);
    else if (!strncmp(shell->cmds->av[0], "pwd", 4))
        bin_pwd(shell);
    else if (!strncmp(shell->cmds->av[0], "env", 4))
        bin_env(shell);
    else if (!strncmp(shell->cmds->av[0], "exit", 5))
        bin_exit(shell);
    else
        execute_simple_command(shell);
}
// void execute_multiple_cmds(t_shell *shell)
// {
// }

// char *get_path(t_shell *shell)
// {
//     char **arr;
//     char *paths;
//     int i;

//     i = 0;
//     paths = getenv("PATH");
//     if (!paths)
//     {
//         shell->exit_status = 127;
//         return (NULL);
//     }
//     paths = ft_split(paths, ":");
//     if (!paths)
//     {
//         shell->exit_status = 1;
//         printf("minishell: malloc: cannot allocate memory\n");
//         return (NULL);
//     }
//     while (paths[i])
//     {
//         i++;
//     }
// }

void execution(t_shell *shell)
{
    t_command *cmd;
    int count;

    cmd = shell->cmds;
    count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    if (count == 1)
        execute_one_cmd(shell);
    else if (count > 1)
        execute_multiple_cmds(shell);
}

int main()
{
    t_shell shell;
    // memset(&shell, 0, sizeof(t_shell));
    execution(&shell);
}
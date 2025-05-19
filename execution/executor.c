#include "../includes/minishell.h"

static void get_exit_code(int status, t_shell *shell)
{
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
}

int execute_builtin(t_command *cmd, t_shell *shell)
{
    if (!strcmp(cmd->av[0], "echo"))
        return (bin_echo(cmd, shell), 1);
    else if (!strcmp(cmd->av[0], "cd"))
        return (bin_cd(cmd, shell), 1);
    else if (!strcmp(cmd->av[0], "export"))
        return (bin_export(cmd, shell), 1);
    else if (!strcmp(cmd->av[0], "unset"))
        return (bin_unset(cmd, shell), 1);
    else if (!strcmp(cmd->av[0], "pwd"))
        return (bin_pwd(shell), 1);
    else if (!strcmp(cmd->av[0], "env"))
        return (bin_env(cmd, shell), 1);
    else if (!strcmp(cmd->av[0], "exit"))
        return (bin_exit(cmd, shell), 1);
    return (0);
}
void exec_child(t_command *cmd, t_shell *shell)
{
    char *full_cmd;

    if (!cmd->av || !cmd->av[0])
        exit(0);
    full_cmd = get_path1(cmd, shell);
    if (!full_cmd)
        exit(shell->exit_status);
    execve(full_cmd, cmd->av, shell->envp);
    perror("execve");
    //restore_fds(shell->in_fd_b, shell->out_fd_b);// ? not sure if needed
    exit(127);
}
void execute_one_cmd(t_command *cmd, t_shell *shell)
{
    pid_t child;
    int status;

    if (open_files(cmd->redirs, shell))
    {
        shell->exit_status = 1;
        return;
    }
    if (execute_builtin(cmd, shell))
        return;
    child = fork();
    if (child == -1)
        return (perror("fork error"));
    if (child == 0)
        exec_child(cmd, shell);
    waitpid(child, &status, 0);
    get_exit_code(status, shell);
}

void execution(t_shell *shell)
{
    t_command *cmd;
    int count;

    in_out_backup(shell); // ! put it inside main
    handle_heredoc(cmd, shell);
    cmd = shell->cmds;
    count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    if (count == 1)
    {
        execute_one_cmd(shell->cmds, shell);
        close_files(shell->cmds->redirs);
        restore_fds(shell->in_fd_b, shell->out_fd_b);
    }
    // else if (count > 1)
    //     execute_multiple_cmds(shell);
}
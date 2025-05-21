#include "../includes/minishell.h"

static void wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
    int i;
    int status;

    i = 0;
    while (i < cmd_count + 1)
        waitpid(pids[i++], &status, 0);
    get_exit_code(status, shell);
}
static void exec_child(t_command *cmd, t_shell *shell)
{
    char *full_cmd;

    if (!cmd->av || !cmd->av[0])
        exit(0);
    if (open_files(cmd->redirs, shell))
        exit(1);
    full_cmd = get_path1(cmd->av[0], shell);
    if (!full_cmd)
        exit(shell->exit_status);
    if (execute_builtin(cmd, shell))
        exit(shell->exit_status);
    execve(full_cmd, cmd->av, shell->envs);
    perror("execve");
    exit(1);
}

void execute_multiple_cmds(int count, t_command *cmd, t_shell *shell)
{
    pid_t *pids;
    int fds[2];
    int status;
    int prev_fd;
    int cmd_count;

    cmd_count = -1;
    prev_fd = -1;
    pids = malloc(sizeof(pid_t) * (count));
    if (!pids)
        return custom_err("malloc", NULL, 1, shell);
    // add_to_allocator(pids, &shell->gc); // ? uncomment this 
    while (cmd)
    {
        if (cmd->next)
            if (pipe(fds) == -1)
                return (pipe_err(shell));
        pids[++cmd_count] = fork();
        if (pids[cmd_count] == -1)
            return (custom_err(strerror(errno), "pipe error", 1, shell));
        if (pids[cmd_count] == 0)
        {
            if (prev_fd != -1)
                dup_in(prev_fd, shell);
            if (cmd->next)
            {
                dup_out(fds[1], shell);
                close(fds[0]);
            }
            exec_child(cmd, shell);
        }
        else
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmd->next)
            {
                close(fds[1]);
                prev_fd = fds[0];
            }
        }
        restore_fds(shell->in_fd_b, shell->out_fd_b);
        cmd = cmd->next;
    }
    wait_for_children(pids, cmd_count, shell);
}
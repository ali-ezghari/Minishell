#include "../includes/minishell.h"

void clean_up_fds(int count, int *fds)
{
    int i;

    i = 0;
    while (i < count)
        close(fds[i++]);
}

void dup_in(int fd, t_shell *shell)
{
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2 error");
        exit(1);
    }
    close(fd);
}
void dup_out(int fd, t_shell *shell)
{
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2 error");
        exit(1);
    }
    close(fd);
}
void restore_fds(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
}
void in_out_backup(t_shell *shell)
{
    shell->in_fd_b = dup(STDIN_FILENO);
    shell->out_fd_b = dup(STDOUT_FILENO);
}
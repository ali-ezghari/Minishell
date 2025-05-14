#include "../includes/minishell.h"

void bin_pwd(t_shell *shell)
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("getcwd error");
        shell->exit_status = 1;
        return;
    }
    printf("%s\n", path);
    free(path);
    shell->exit_status = 0;
}
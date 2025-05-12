#include "../includes/minishell.h"

void bin_pwd()
{
    char *path;

    path = getcwd(NULL, 0);
    if (!path)
    {
        perror("getcwd error");
        // need to save the exit status : 1
        return;
    }
    printf("%s\n", path);
    free(path);
    // need to save the exit status : 0
}
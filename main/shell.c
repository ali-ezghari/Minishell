#include "../includes/minishell.h"

void shell_loop(t_shell *shell)
{
    char *line;

    while (1)
    {
        line = readline("Minishell>");
        if (!line || !line[0])
        {
            return;
        }
        add_history(line);
        parsing(shell);
        execution(shell);
        free_all(&shell->allocator);
        shell->allocator = NULL;
        free(line);
    }
    clear_history();
}

int main(int argc, char *argv[], char *envp[])
{
    t_shell shell;

    shell.allocator = NULL;
    in_out_backup(&shell);
    shell_loop(&shell);
    return (0);
}

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

int main()
{
    t_shell shell;

    shell.allocator = NULL;
    shell_loop(&shell);
    return (0);
}
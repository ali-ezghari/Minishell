#include "../includes/minishell.h"

static char *handle_direct_path(char *cmd, t_shell *shell)
{
    if (access(cmd, F_OK) != 0)
        return (custom_cmd_err(strerror(errno), cmd, 127, shell), NULL);
    if (access(cmd, X_OK) != 0)
        return (custom_cmd_err(strerror(errno), cmd, 126, shell), NULL);
    return (ft_strdup(cmd));
}
static char *get_path2(char *cmd, char **paths_arr, t_shell *shell)
{
    char *full_cmd;
    int i;

    i = 0;
    while (paths_arr[i])
    {
        full_cmd = ft_strjoin_three(paths_arr[i], "/", cmd);
        add_to_allocator(full_cmd, &shell->gc);
        if (!full_cmd)
        {
            shell->exit_status = 1;
            return (NULL);
        }
        if (access(full_cmd, X_OK) == 0)
            return (full_cmd);
        i++;
    }
    custom_err("command not found", cmd, 127, shell);
    return (NULL);
}
char *get_path1(char *cmd, t_shell *shell)
{
    char **paths_arr;
    char *paths_str;
    char *full_cmd;

    if (ft_strlen(cmd) == 0)
        return (custom_err("Command not found", "''", 127, shell), NULL);
    paths_str = ft_getenv("PATH", shell->envp);
    if (!paths_str)
        return (custom_err("Command not found", cmd, 127, shell), NULL);
    if (ft_strchr(cmd, '/'))
        return (handle_direct_path(cmd, shell));
    paths_arr = ft_split(paths_str, ':');
    if (!paths_arr)
    {
        shell->exit_status = 1;
        return (NULL);
    }
    add_string_array_to_allocator(paths_arr, &shell->gc);
    full_cmd = get_path2(cmd, paths_arr, shell);
    return (full_cmd);
}
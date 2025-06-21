#include "../includes/minishell.h"

int print_synerror(const char *content)
{
    ft_putstr_fd("syntax error near unexpected token `", 2);
    ft_putstr_fd((char *)content, 2);
    ft_putstr_fd("'\n", 2);
    return (1);
}

int check_unclosed_quotes(const char *line)
{
    int single = 0, doubleq = 0;
    while (*line)
    {
        if (*line == '\'' && doubleq % 2 == 0)
            single++;
        else if (*line == '"' && single % 2 == 0)
            doubleq++;
        line++;
    }
    if (single % 2 || doubleq % 2)
    {
        ft_putstr_fd("Missing closing quote!!\n", 2);
        return (1);
    }
    return (0);
} 
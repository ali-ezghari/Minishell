#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Use your ft_strdup from libft but allocate with gc
static char *gc_strdup(const char *s, t_allocator **gc)
{
    char *copy;
    size_t len = ft_strlen(s);

    copy = ft_malloc(len + 1, gc);
    if (!copy)
        return NULL;
    ft_memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

// Create a new redirection node using allocator gc
static t_redir *new_redir(t_token_type type, char *filename, t_allocator **gc)
{
    t_redir *redir = ft_malloc(sizeof(t_redir), gc);
    if (!redir)
        return NULL;
    redir->type = type;
    redir->filename = gc_strdup(filename, gc);
    if (!redir->filename)
        return NULL;
    redir->next = NULL;
    return redir;
}

// Append redirection to the redir list
static void add_redir(t_redir **redir_list, t_redir *new)
{
    if (!*redir_list)
        *redir_list = new;
    else
    {
        t_redir *tmp = *redir_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

static void add_arg(char ***av, char *value, t_allocator **gc)
{
    int count = 0;
    if (*av)
    {
        // count the number of existing args
        while ((*av)[count])
            count++;
    }

    // allocate new array with space for one more arg + NULL terminator
    char **new_av = ft_malloc(sizeof(char *) * (count + 2), gc);
    if (!new_av)
        return;

    // copy old pointers manually 
    if (count > 0)
    {
        int i = 0;
        while (i < count)
        {
            new_av[i] = (*av)[i];
            i++;
        }
    }

    new_av[count] = gc_strdup(value, gc);
    new_av[count + 1] = NULL;

    // No free old *av because garbage collector will free it later
    *av = new_av;
}


// Create a new command node with gc allocator
static t_command *new_command(t_allocator **gc)
{
    t_command *cmd = ft_malloc(sizeof(t_command), gc);
    if (!cmd)
        return NULL;
    cmd->av = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    cmd->is_builtin = 0;
    return cmd;
}

// Parse tokens into linked list of commands using gc allocator
t_command *parse_tokens(t_token *tok, t_allocator **gc)
{
    t_command *head = NULL;
    t_command *cur = NULL;

    while (tok)
    {
        if (!cur)
        {
            cur = new_command(gc);
            if (!head)
                head = cur;
        }
        if (tok->type == T_WORD)
        {
            add_arg(&cur->av, tok->value, gc);
        }
        else if (tok->type == T_REDIRECT_IN || tok->type == T_REDIRECT_OUT
            || tok->type == T_APPEND || tok->type == T_HEREDOC)
        {
            if (tok->next)
            {
                t_redir *r = new_redir(tok->type, tok->next->value, gc);
                add_redir(&cur->redirs, r);
                tok = tok->next; // skip filename
            }
        }
        else if (tok->type == T_PIPE)
        {
            t_command *next = new_command(gc);
            cur->next = next;
            cur = next;
        }
        tok = tok->next;
    }
    return head;
}

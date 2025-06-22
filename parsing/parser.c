/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:32:26 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:35:59 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/utils.h"
#include "../libft/libft.h"

static int is_redirection_type(t_token_type type)
{
    return (type == T_REDIRECT_IN || type == T_REDIRECT_OUT
        || type == T_APPEND || type == T_HEREDOC);
}

static char *gc_strdup(const char *s, t_allocator **gc)
{
    char *copy;
    size_t len = ft_strlen(s);

    if (!s || !gc)
        return NULL;
    copy = ft_malloc(len + 1, gc);
    if (!copy)
        return NULL;
    ft_memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

static t_redir *new_redir(t_token_type type, char *filename, t_allocator **gc)
{
    t_redir *redir;

    if (!filename || !gc)
        return NULL;
    redir = ft_malloc(sizeof(t_redir), gc);
    if (!redir)
        return NULL;
    redir->type = type;
    redir->filename = gc_strdup(filename, gc);
    if (!redir->filename)
        return NULL;
    redir->next = NULL;
    redir->fd = -1;
    return redir;
}

static void add_redir(t_redir **redir_list, t_redir *new)
{
    t_redir *tmp;

    if (!redir_list || !new)
        return;
    if (!*redir_list)
        *redir_list = new;
    else
    {
        tmp = *redir_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

static void add_arg(char ***av, char *value, t_allocator **gc)
{
    int count = 0;
    char **new_av;
    int i;

    if (!av || !value || !gc)
        return;
    if (*av)
    {
        while ((*av)[count])
            count++;
    }

    new_av = ft_malloc(sizeof(char *) * (count + 2), gc);
    if (!new_av)
        return;
    i = 0;
    while (i < count)
    {
        new_av[i] = (*av)[i];
        i++;
    }

    new_av[count] = gc_strdup(value, gc);
    new_av[count + 1] = NULL;

    *av = new_av;
}


static t_command *new_command(t_allocator **gc)
{
    t_command *cmd;

    if (!gc)
        return NULL;
    cmd = ft_malloc(sizeof(t_command), gc);
    if (!cmd)
        return NULL;
    cmd->av = NULL;
    cmd->redirs = NULL;
    cmd->next = NULL;
    return cmd;
}

t_command *parse_tokens(t_token *tok, t_shell *shell)
{
    t_command *head = NULL;
    t_command *cur = NULL;

    if (!tok || !shell)
        return NULL;
    if (catch_syntax_errors(tok, shell))
        return NULL;

    head = NULL;
    cur = NULL;
    while (tok)
    {
        if (!cur)
        {
            cur = new_command(&shell->gc);
            if (!cur)
                return NULL;
            if (!head)
                head = cur;
        }

        if (tok->type == T_WORD || tok->quoted)
        {
            char *value = tok->value;
            if (tok->quoted)
            {
                // Remove surrounding quotes but preserve the content
                size_t len = strlen(value);
                if (len >= 2)
                {
                    char *unquoted = ft_substr(value, 1, len - 2);
                    add_arg(&cur->av, unquoted, &shell->gc);
                }
                else
                {
                    add_arg(&cur->av, "", &shell->gc);
                }
            }
            else
            {
                add_arg(&cur->av, value, &shell->gc);
            }
        }
        else if (is_redirection_type(tok->type) && !tok->quoted)
        {
            if (tok->next)
            {
                t_redir *r = new_redir(tok->type, tok->next->value, &shell->gc);
                if (!r)
                    return NULL;
                add_redir(&cur->redirs, r);
                tok = tok->next;
            }
        }
        else if (tok->type == T_PIPE && !tok->quoted)
        {
            t_command *next = new_command(&shell->gc);
            if (!next)
                return NULL;
            cur->next = next;
            cur = next;
        }
        tok = tok->next;
    }
    return head;
}

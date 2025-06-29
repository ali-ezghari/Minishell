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

static int is_redirection_type(t_token_type type)
{
    return (type == T_REDIRECT_IN || type == T_REDIRECT_OUT
        || type == T_APPEND || type == T_HEREDOC);
}

static char *gc_strdup(const char *s)
{
    char *copy;
    size_t len = ft_strlen(s);

    if (!s)
        return NULL;
    copy = ft_malloc(len + 1);
    if (!copy)
        return NULL;
    ft_memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

static t_redir *new_redir(t_token_type type, char *filename)
{
    t_redir *redir;

    if (!filename)
        return NULL;
    redir = ft_malloc(sizeof(t_redir));
    if (!redir)
        return NULL;
    redir->type = type;
    redir->filename = gc_strdup(filename);
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

static void add_arg(char ***av, char *value)
{
    int count = 0;
    char **new_av;
    int i;

    if (!av || !value)
        return;
    if (*av)
    {
        while ((*av)[count])
            count++;
    }

    new_av = ft_malloc(sizeof(char *) * (count + 2));
    if (!new_av)
        return;
    i = 0;
    while (i < count)
    {
        new_av[i] = (*av)[i];
        i++;
    }

    new_av[count] = gc_strdup(value);
    new_av[count + 1] = NULL;

    *av = new_av;
}

static t_command *new_command(void)
{
    t_command *cmd;

    cmd = ft_malloc(sizeof(t_command));
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
            cur = new_command();
            if (!cur)
                return NULL;
            if (!head)
                head = cur;
        }

        if (tok->type == T_WORD || tok->quoted)
        {
            char *value = tok->value;
            if (tok->quoted && ft_strchr(value, '$'))
            {
                char *expanded = handle_exp(value, shell, shell->exit_status);
                if (!expanded)
                    return NULL;
                add_arg(&cur->av, expanded);
                free(expanded);
            }
            else
            {
                add_arg(&cur->av, value);
            }
            tok = tok->next;
            continue;
        }
        else if (is_redirection_type(tok->type) && !tok->quoted)
        {
            if (tok->next)
            {
                t_redir *r = new_redir(tok->type, tok->next->value);
                if (!r)
                    return NULL;
                add_redir(&cur->redirs, r);
                tok = tok->next;
            }
        }
        else if (tok->type == T_PIPE && !tok->quoted)
        {
            t_command *next = new_command();
            if (!next)
                return NULL;
            cur->next = next;
            cur = next;
        }
        tok = tok->next;
    }
    return head;
}
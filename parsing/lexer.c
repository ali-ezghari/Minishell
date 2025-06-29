/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:32:26 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:35:59 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token_type get_token_type(const char *s)
{
    if (!s)
        return T_WORD;
    if (ft_strcmp(s, "|") == 0)
        return T_PIPE;
    else if (ft_strcmp(s, "<") == 0)
        return T_REDIRECT_IN;
    else if (ft_strcmp(s, ">") == 0)
        return T_REDIRECT_OUT;
    else if (ft_strcmp(s, ">>") == 0)
        return T_APPEND;
    else if (ft_strcmp(s, "<<") == 0)
        return T_HEREDOC;
    return T_WORD;
}

t_token *new_token(char *val)
{
    t_token *tok;
    char    *copy;
    int     quoted = 0;

    if (!val)
        return NULL;
    // Special case for $? which should never be quoted
    if (strcmp(val, "$?") == 0)
    {
        quoted = 0;
    }
    else if ((val[0] == '"' || val[0] == '\'') && val[strlen(val)-1] == val[0])
    {
        quoted = 1;
    }
    tok = ft_malloc(sizeof(t_token));
    if (!tok)
        return NULL;
    copy = ft_malloc(ft_strlen(val) + 1);
    if (!copy)
        return NULL;
    ft_strlcpy(copy, val, ft_strlen(val) + 1);
    tok->value = copy;
    tok->type = get_token_type(copy);
    tok->next = NULL;
    tok->quoted = quoted;
    return tok;
}

t_token *build_lexed_tokens(char **token_array)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *new;
    int     i = 0;

    if (!token_array)
        return NULL;
    while (token_array[i])
    {
        new = new_token(token_array[i]);
        if (!new)
            return NULL;
        if (!head)
            head = new;
        else
            tail->next = new;
        tail = new;
        i++;
    }
    return head;
}

/*

int main(void)
{
    t_allocator *gc = NULL;

    char *line = readline("minishell$ ");
    char **arr = tokenize(line);
    t_token *tokens = build_lexed_tokens(arr, &gc);
    t_token *tmp = tokens;

    while (tmp)
    {
        printf("Token: %-10s Type: %d\n", tmp->value, tmp->type);
        tmp = tmp->next;
    }

    free_token_array(arr);
    free(line);
    free_all(&gc);
    return 0;
}
*/
#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <string.h> // for strcmp

t_token_type get_token_type(const char *s)
{
    if (strcmp(s, "|") == 0)
        return T_PIPE;
    else if (strcmp(s, "<") == 0)
        return T_REDIRECT_IN;
    else if (strcmp(s, ">") == 0)
        return T_REDIRECT_OUT;
    else if (strcmp(s, ">>") == 0)
        return T_APPEND;
    else if (strcmp(s, "<<") == 0)
        return T_HEREDOC;
    return T_WORD;
}

// Allocate token and its value using garbage collector allocator
t_token *new_token(char *val, t_allocator **gc)
{
    t_token *tok;
    char    *copy;

    tok = ft_malloc(sizeof(t_token), gc);
    if (!tok)
        return NULL;

    copy = ft_malloc(strlen(val) + 1, gc);
    if (!copy)
        return NULL;

    strcpy(copy, val);
    tok->value = copy;
    tok->type = get_token_type(val);
    tok->next = NULL;
    return tok;
}

// Build linked list of tokens with allocations tracked in gc
t_token *build_lexed_tokens(char **token_array, t_allocator **gc)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *new;
    int     i = 0;

    while (token_array[i])
    {
        new = new_token(token_array[i], gc);
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
// Example test main (make sure to free_all(gc) after)
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

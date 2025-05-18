#include "../includes/minishell.h"
#include "../includes/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define IS_OPERATOR(c) ((c) == '|' || (c) == '<' || (c) == '>')
#define MAX_TOKENS 512

char *ft_strndup(const char *s, size_t n) 
{
    char *dup;
    size_t i;

    dup = malloc(n + 1);
    if (!dup)
        return NULL;
    i = 0;
    while (i < n)
    {
        dup[i] = s[i];
        i++;
    }
    dup[n] = '\0';
    return dup;
}

void skip_spaces(const char **str)
{
    while (**str && (**str == ' ' || (**str >= 9 && **str <= 13)))
        (*str)++;
}

char *get_quoted_token(const char **str)
{
    char quote;
    const char *start;
    size_t len;

    quote = **str;
    (*str)++;
    start = *str;
    while (**str && **str != quote)
        (*str)++;
    len = *str - start;
    if (**str == quote)
        (*str)++;
    return ft_strndup(start, len);
}

char *get_operator_token(const char **str, t_allocator **gc)
{
    char *token = ft_malloc(3, gc);
    if (!token)
        return NULL;

    token[0] = **str;
    token[1] = '\0';
    token[2] = '\0';

    if ((**str == '<' || **str == '>') && *(*str + 1) == **str)
    {
        token[1] = **str;
        (*str) += 2;
    }
    else
        (*str)++;
    return token;
}

char *get_env_var_token(const char **str)
{
    const char *start;
    size_t len;
    char *var_name;
    char *value;

    (*str)++;
    start = *str;
    while (**str && (ft_isalnum(**str) || **str == '_'))
        (*str)++;
    len = *str - start;
    var_name = ft_strndup(start, len);
    value = getenv(var_name);
    free(var_name);
    if (!value)
        return ft_strdup("");
    return ft_strdup(value);
}

char *get_word_token(const char **str)
{
    const char *start;

    start = *str;
    while (**str
        && !(**str == ' ' || (**str >= 9 && **str <= 13))
        && !IS_OPERATOR(**str)
        && **str != '"'
        && **str != '\''
        && **str != '$')
    {
        (*str)++;
    }
    return ft_strndup(start, *str - start);
}

char **tokenize(const char *input, t_allocator **gc)
{
    const char *p = input;
    char **tokens = ft_malloc(sizeof(char *) * MAX_TOKENS, gc);
    int i = 0;

    if (!tokens)
        return NULL;

    while (*p)
    {
        skip_spaces(&p);
        if (!*p)
            break;

        if (*p == '"' || *p == '\'')
            tokens[i++] = get_quoted_token(&p);
        else if (*p == '$')
            tokens[i++] = get_env_var_token(&p);
        else if (IS_OPERATOR(*p))
            tokens[i++] = get_operator_token(&p, gc);
        else
            tokens[i++] = get_word_token(&p);
    }

    tokens[i] = NULL;
    return tokens;
}

/*int main(int ac, char **av)
{
    char *cmd = av[1];
    char **tokens;
    int i;

    tokens = tokenize(cmd);
    i = 0;
    while (tokens[i])
    {
        printf("Token %d: [%s]\n", i, tokens[i]);
        free(tokens[i]);
        i++;
    }
    free(tokens);
    return 0;
}*/

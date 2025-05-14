#include "includes/minishell.h"


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

void skip_spaces(const char **str) //white spaces friendly
{
    while (**str && isspace(**str))  //i will include ft_isspace later
        (*str)++;
}

char *get_quoted_token(const char **str) //spaces friendly
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
    return strndup_custom(start, len); //ret str without quotes
}

//tokens : |, >, <, >>, <<
char *get_operator_token(const char **str)
{
    char *token;

    token = malloc(3);
    if (!token)
        return NULL;
    token[0] = **str; //op char
    token[1] = '\0'; //for now
    token[2] = '\0'; //just in case

    if ((**str == '<' || **str == '>') && *(*str + 1) == **str)
    {
        token[1] = **str; //dup op
        (*str) += 2; //skip both
    }
    else
        (*str)++; //else skip 1
    return token;
}
//handls ex:$var and rep with getenv()
char *get_env_var_token(const char **str) 
{
    const char *start;
    size_t len;
    char *var_name;
    char *value;

    (*str)++; // skip '$'
    start = *str;
    while (**str && (isalnum(**str) || **str == '_'))
        (*str)++;
    len = *str - start;
    var_name = strndup_custom(start, len);
    value = getenv(var_name);
    free(var_name);
    if (!value)
        return strdup(""); //ret empty str if not found
    return strdup(value);
}
//plain word
char *get_word_token(const char **str)
{
    const char *start;

    start = *str;
    while (**str && !isspace(**str) && !IS_OPERATOR(**str) && **str != '"' && **str != '\'' && **str != '$')
        (*str)++;
    return strndup_custom(start, *str - start); //ret extracted word
}

char **tokenize(const char *input) //main loop
{
    const char *p;
    char **tokens;
    int i;

    p = input;
    tokens = malloc(sizeof(char *) * MAX_TOKENS);
    if (!tokens)
        return NULL;
    i = 0;

    while (*p)
    {
        skip_spaces(&p);
        if (!*p)
            break;

        if (*p == '"' || *p == '\'')//quotes
            tokens[i++] = get_quoted_token(&p);
        else if (*p == '$')//env var
            tokens[i++] = get_env_var_token(&p);
        else if (IS_OPERATOR(*p))//operators
            tokens[i++] = get_operator_token(&p);
        else //regular word
            tokens[i++] = get_word_token(&p);
    }

    tokens[i] = NULL;
    return tokens;
}

int main(void)
{
    char *cmd = "echo $USER | grep \"test me\" >> outfile";
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
}

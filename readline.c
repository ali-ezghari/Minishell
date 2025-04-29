#include "minishell.h"

int main(void)
{
    char *input;
    char **tokens;
    int i;
    
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        
        if (*input)
            add_history(input);
        
        tokens = input_split(input);
        
        i = 0;
        while (tokens && tokens[i])
        {
            printf("Token[%d]: %s\n", i, tokens[i]);
            i++;
        }
        i = 0;
        while (tokens && tokens[i])
        {
            free(tokens[i]);
            i++;
        }
        
        free(tokens);
        free(input);
    }
    
    printf("exit\n");
    return 0;
}
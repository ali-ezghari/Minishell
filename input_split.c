/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:49:33 by fatouil           #+#    #+#             */
/*   Updated: 2025/04/29 15:14:18 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int count_words(char *str)
{
    int wd_count;
    int in_wd;

    wd_count = 0;
    in_wd = 0;
    while (*str) 
    {
        if (*str != ' ' && !in_wd)
        {
            in_wd = 1;
            wd_count++;
        }
        else if (*str == ' ')
            in_wd = 0;
        str++;
    }
    return (wd_count);
}

static char *ft_strndup(const char *s, size_t n)
{
    char *result;
    size_t i;

    if (!s)
        return NULL;
    
    result = (char *)malloc(n + 1);
    if (!result)
        return NULL;
    
    i = 0;
    while (i < n && s[i])
    {
        result[i] = s[i];
        i++;
    }
    result[i] = '\0';
    
    return result;
}

static char *next_word(char **str)
{
    char *start;
    int len = 0;

    while (**str == ' ')
        (*str)++;
    start = *str;
    while (**str && **str != ' ')
    {
        len++;
        (*str)++;
    }
    return ft_strndup(start, len);
}

char **input_split(char *input)
{
    int i;
    int wd_count;
    char **tokens;

    i = 0;
    wd_count = count_words(input);
    tokens = malloc(sizeof(char *) * (wd_count + 1));
    if (!tokens)
        return NULL;
    while (i < wd_count)
        tokens[i++] = next_word(&input);
    tokens[i] = NULL;
    return tokens;
}

/*int main(void)
{
    char *line = "  test1 tst2     555            last";
    
    char **tokens = input_split(line);
    
    for (int i = 0; tokens && tokens[i]; i++)
        printf("Token[%d]: %s\n", i, tokens[i]);
    
    for (int i = 0; tokens && tokens[i]; i++)
        free(tokens[i]);
    free(tokens);
    // free(line);
    
    return 0;
}*/
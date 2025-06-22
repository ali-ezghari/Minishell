/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
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

#define IS_OPERATOR(c) ((c) == '|' || (c) == '<' || (c) == '>')
#define MAX_TOKENS 512

char	*ft_strndup(const char *s, size_t n, t_allocator **gc)
{
	char	*dup;
	size_t	i;

	if (!s || !gc)
		return (NULL);
	dup = ft_malloc(n + 1, gc);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[n] = '\0';
	return (dup);
}

void	skip_spaces(const char **str)
{
	if (!str || !*str)
		return ;
	while (**str && (**str == ' ' || (**str >= 9 && **str <= 13)))
		(*str)++;
}

char	*get_quoted_token(const char **str, t_allocator **gc)
{
	char		quote;
	const char	*start;
	size_t		len;
	char		*token;

	if (!str || !*str || !**str || !gc)
		return NULL;
	quote = **str;
	(*str)++;
	start = *str;
	while (**str && **str != quote)
		(*str)++;
	len = *str - start;
	if (**str == quote)
		(*str)++;
	// Create token including the quotes
	token = ft_malloc(len + 3, gc); // +3 for two quotes and null terminator
	if (!token)
		return NULL;
	token[0] = quote;
	ft_memcpy(token + 1, start, len);
	token[len + 1] = quote;
	token[len + 2] = '\0';
	return token;
}

char	*get_operator_token(const char **str, t_allocator **gc)
{
	char	*token;

	if (!str || !*str || !**str || !gc)
		return (NULL);
	token = ft_malloc(3, gc);
	if (!token)
		return (NULL);
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
	return (token);
}

char	*get_env_var_token(const char **str, t_allocator **gc, t_shell *shell)
{
	const char	*start;
	size_t		len;
	char		*var_name;
	char		*value;
	char		*result;

	if (!str || !*str || !**str || !gc)
		return (NULL);
	(*str)++;
	// Handle $? case (exit status)
	if (**str == '?')
	{
		(*str)++;
		if (shell)
		{
			result = ft_itoa(shell->exit_status);
			if (result)
			{
				char *gc_result = ft_malloc(ft_strlen(result) + 1, gc);
				if (gc_result)
				{
					ft_strlcpy(gc_result, result, ft_strlen(result) + 1);
					free(result);
					return gc_result;
				}
				free(result);
			}
			return NULL;
		}
		else
		{
			result = ft_itoa(0);
			if (result)
			{
				char *gc_result = ft_malloc(ft_strlen(result) + 1, gc);
				if (gc_result)
				{
					ft_strlcpy(gc_result, result, ft_strlen(result) + 1);
					free(result);
					return gc_result;
				}
				free(result);
			}
			return NULL;
		}
	}
	// Handle lone $ case
	if (!**str || **str == ' ' || **str == '\'' || **str == '"' || **str == '$')
	{
		char *gc_result = ft_malloc(2, gc);
		if (gc_result)
		{
			gc_result[0] = '$';
			gc_result[1] = '\0';
			return gc_result;
		}
		return NULL;
	}
	start = *str;
	while (**str && (ft_isalnum(**str) || **str == '_'))
		(*str)++;
	len = *str - start;
	var_name = ft_strndup(start, len, gc);
	if (!var_name)
		return (NULL);
	value = getenv(var_name);
	if (!value)
	{
		char *gc_result = ft_malloc(1, gc);
		if (gc_result)
		{
			gc_result[0] = '\0';
			return gc_result;
		}
		return NULL;
	}
	result = ft_malloc(ft_strlen(value) + 1, gc);
	if (result)
	{
		ft_strlcpy(result, value, ft_strlen(value) + 1);
		return result;
	}
	return NULL;
}

char	*get_word_token(const char **str, t_allocator **gc)
{
	const char	*start;

	if (!str || !*str || !**str || !gc)
		return (NULL);
	start = *str;
	while (**str && !(**str == ' ' || (**str >= 9 && **str <= 13))
		&& !IS_OPERATOR(**str) && **str != '"' && **str != '\''
		&& **str != '$')
		(*str)++;
	return (ft_strndup(start, *str - start, gc));
}

char	**tokenize(const char *inp, t_allocator **gc, t_shell *shell)
{
	const char	*p;
	char		**tokens;
	int			i;

	if (!inp || !gc)
		return (NULL);
	p = inp;
	tokens = ft_malloc(sizeof(char *) * MAX_TOKENS, gc);
	if (!tokens)
		return (NULL);
	i = 0;
	while (*p)
	{
		skip_spaces(&p);
		if (!*p)
			break ;
		if (*p == '"' || *p == '\'')
			tokens[i++] = get_quoted_token(&p, gc);
		else if (*p == '$')
			tokens[i++] = get_env_var_token(&p, gc, shell);
		else if (IS_OPERATOR(*p))
			tokens[i++] = get_operator_token(&p, gc);
		else
			tokens[i++] = get_word_token(&p, gc);
	}
	tokens[i] = NULL;
	return (tokens);
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

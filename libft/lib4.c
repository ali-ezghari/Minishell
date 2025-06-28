#include "../includes/minishell.h"

static char	**mem_error(char **tab)
{
	int	i = 0;

	while (tab[i])
		free(tab[i++]);
	free(tab);
	return (NULL);
}


static int	count_words(char *str, char sep)
{
	int		i = 0, in_word = 0, count = 0;

	while (str[i] != '\0')
	{
		if (!in_word && str[i] != sep)
		{
			in_word = 1;
			count++;
		}
		else if (str[i] == sep)
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*copy(char *str, int *start, char sep)
{
	char	*dest;
	int		i = 0, len = 0;

	while (str[*start] == sep && str[*start])
		(*start)++;
	while (str[*start + len] != sep && str[*start + len])
		len++;
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	while (i < len && str[*start])
		dest[i++] = str[(*start)++];
	dest[i] = '\0';
	return (dest);
}

char	**ft_split(char *str, char sep)
{
	int		size, i = 0, pos = 0;
	char	**ptr;

	if (!str)
		return (NULL);
	size = count_words(str, sep);
	ptr = malloc(sizeof(char *) * (size + 1));
	if (!ptr)
		return (NULL);
	ptr[size] = NULL;
	while (i < size)
	{
		ptr[i] = copy(str, &pos, sep);
		if (!ptr[i])
			return (mem_error(ptr));
		i++;
	}
	return (ptr);
}
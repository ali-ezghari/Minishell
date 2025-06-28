#include "../includes/minishell.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i = 0;
	while (s[i] != '\0' && s[i] != (unsigned char)c)
		i++;
	if ((unsigned char)c == s[i])
		return ((char *)s + i);
	return (NULL);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i = 0;
	size_t	slen = ft_strlen(src);

	if (size == 0)
		return (slen);
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (slen);
}

size_t	ft_strlen(const char *s)
{
	size_t	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i = 0, j, dlen = ft_strlen(dst), slen = ft_strlen(src);

	if (!dst && size == 0)
		return (slen);
	if (size <= dlen)
		return (size + slen);
	j = dlen;
	while (i < (size - dlen - 1) && src[i] != '\0')
		dst[j++] = src[i++];
	dst[j] = '\0';
	return (dlen + slen);
}

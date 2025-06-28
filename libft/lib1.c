#include "../includes/minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (s1 == NULL && s2 == NULL)
		return (0);
	while ((i < n) && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

t_allocator	*ft_lstlast(t_allocator *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstclear(t_allocator **lst, void (*del)(void*))
{
	t_allocator	*current;
	t_allocator	*next;

	if (!*lst || !del)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		del(current->ptr);
		free(current);
		current = next;
	}
	*lst = NULL;
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}
int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
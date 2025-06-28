/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:25:41 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:26:26 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "minishell.h"

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
char	**ft_split(char *str, char sep);
int		ft_isascii(int c);
int		ft_isalnum(int c);
int		ft_isalpha(int c);  
int		ft_isdigit(int c);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_atoi(const char *str);
char	*ft_itoa(int n);

typedef struct s_allocator
{
	void				*ptr;
	struct s_allocator	*next;
}	t_allocator;

t_allocator	*ft_lstlast(t_allocator *lst);
void		ft_lstadd_back(t_allocator **lst, t_allocator *new);
void		ft_lstclear(t_allocator **lst, void (*del)(void*));
void	*ft_malloc(size_t size);
void	add_to_allocator(void *ptr);
t_allocator	*ft_lstnew_custom(void	*ptr);
#endif


// garbage.h
#ifndef UTILS_H
#define UTILS_H

#include "minishell.h"

typedef struct s_allocator
{
    void                *ptr;
    struct s_allocator    *next;
}   t_allocator;

void *ft_malloc(size_t size, t_allocator **allocator);
void free_all(t_allocator **allocator);
void add_to_allocator(void *ptr, t_allocator **allocator);


int	ft_strcmp(const char *s1, const char *s2);
#endif

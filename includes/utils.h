#ifndef UTILS_H
#define UTILS_H

#include "minishell.h"

typedef struct s_allocator
{
    void                *ptr;
    struct s_allocator    *next;
}   t_allocator;

void *ft_malloc(size_t size, t_allocator **gc);
void free_all(t_allocator **gc);
void add_to_allocator(void *ptr, t_allocator **gc);
#endif

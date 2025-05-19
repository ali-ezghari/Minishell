#include "../includes/minishell.h"

void add_to_allocator(void *ptr, t_allocator **gc)
{
    t_allocator *new;

    if (!ptr)
        return;
    new = malloc(sizeof(t_allocator));
    if (!new)
        return;
    new->ptr = ptr;
    new->next = *gc;
    *gc = new;
}

void *ft_malloc(size_t size, t_allocator **gc)
{
    void        *ptr;
    t_allocator   *new;

    ptr = malloc(size);
    if (!ptr)
        return (NULL);
    new = malloc(sizeof(t_allocator));
    if (!new)
    {
        free(ptr);
        return (NULL);
    }
    new->ptr = ptr;
    new->next = *gc;
    *gc = new;
    return (ptr);
}

void free_all(t_allocator **gc)
{
    t_allocator *tmp;

    while (*gc)
    {
        free((*gc)->ptr);
        tmp = *gc;
        *gc = (*gc)->next;
        free(tmp);
    }
}

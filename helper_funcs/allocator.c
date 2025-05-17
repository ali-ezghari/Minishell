#include "../includes/utils.h"

void add_to_allocator(void *ptr, t_allocator **allocator)
{
    t_allocator *new;

    if (!ptr)
        return;
    new = malloc(sizeof(t_allocator));
    if (!new)
        return;
    new->ptr = ptr;
    new->next = *allocator;
    *allocator = new;
}

void *ft_malloc(size_t size, t_allocator **allocator)
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
    new->next = *allocator;
    *allocator = new;
    return (ptr);
}

void free_all(t_allocator **allocator)
{
    t_allocator *tmp;

    while (*allocator)
    {
        free((*allocator)->ptr);
        tmp = *allocator;
        *allocator = (*allocator)->next;
        free(tmp);
    }
}

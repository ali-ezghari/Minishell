/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:36:56 by aezghari          #+#    #+#             */
/*   Updated: 2025/06/26 09:53:53 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_to_allocator(void *ptr)
{
	t_allocator	*new;

	if (!ptr)
		return ;
	new = malloc(sizeof(t_allocator));
	if (!new)
		return ;
	new->ptr = ptr;
	ft_lstadd_back(&gc, new);
}

void	*ft_malloc(size_t size)
{
	void		*ptr;
	t_allocator	*new;

	ptr = malloc(size);
	if (!ptr)
		return (allocation_failure(), NULL);
	new = malloc(sizeof(t_allocator));
	if (!new)
		return (allocation_failure(), NULL);
	new->ptr = ptr;
	ft_lstadd_back(&gc, new);
	return (ptr);
}

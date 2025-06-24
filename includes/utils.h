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

# include "minishell.h"

typedef struct s_allocator
{
	void					*ptr;
	struct s_allocator		*next;
}							t_allocator;

void	*ft_malloc(size_t size);
void	add_to_allocator(void *ptr);
#endif

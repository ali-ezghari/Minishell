/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:36:40 by aezghari          #+#    #+#             */
/*   Updated: 2025/06/26 09:54:09 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_setup(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
}
t_allocator	*ft_lstnew_custom(void	*ptr)
{
	t_allocator	*new_node;

	new_node = (t_allocator *)ft_malloc(sizeof(t_allocator));
	if (!new_node)
		return (allocation_failure(), NULL);
	new_node->ptr = ptr;
	new_node->next = NULL;
	return (new_node);
}
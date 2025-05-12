/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <aezghari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:01:37 by aezghari          #+#    #+#             */
/*   Updated: 2024/11/01 08:55:38 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*ft_map_node(void *content, void (*del)(void *), t_list **head)
{
	t_list	*node;

	node = ft_lstnew(content);
	if (!node)
	{
		del(content);
		ft_lstclear(head, del);
		return (NULL);
	}
	return (node);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*node;
	void	*content;

	head = NULL;
	if (!f || !lst || !del)
		return (NULL);
	while (lst)
	{
		content = f(lst->content);
		if (!content)
		{
			ft_lstclear(&head, del);
			return (NULL);
		}
		node = ft_map_node(content, del, &head);
		if (!node)
			return (NULL);
		ft_lstadd_back(&head, node);
		lst = lst->next;
	}
	return (head);
}

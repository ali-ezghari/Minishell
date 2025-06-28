/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:43:23 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/26 18:24:51 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_env(char *key, char *value)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	add_to_allocator(node->key);
	add_to_allocator(node->value);
	node->next = NULL;
	return (node);
}

t_allocator	*newnode(void *content)
{
	t_allocator	*head;

	head = ft_malloc(sizeof(t_allocator));
	if (!head)
		return (NULL);
	head->ptr = content;
	head->next = (NULL);
	return (head);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*joined;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	joined = ft_strjoin(tmp, s3);
	free(tmp);
	return (joined);
}

void	add_string_array_to_allocator(char **arr)
{
	int	i;

	i = 0;
	add_to_allocator(arr);
	while (arr[i])
	{
		add_to_allocator(arr[i]);
		i++;
	}
}

void	allocation_failure(void)
{
	ft_lstclear(&gc, free);
	exit(1);
}

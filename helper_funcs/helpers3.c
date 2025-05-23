/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:43:23 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:45:51 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	add_string_array_to_allocator(char **arr, t_allocator **gc)
{
	int	i;

	i = 0;
	add_to_allocator(arr, gc);
	while (arr[i])
	{
		add_to_allocator(arr[i], gc);
		i++;
	}
}

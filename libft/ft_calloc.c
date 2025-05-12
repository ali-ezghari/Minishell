/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:02:04 by aezghari          #+#    #+#             */
/*   Updated: 2024/11/06 16:44:43 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;

	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	arr = malloc(size * nmemb);
	if (!arr)
		return (NULL);
	ft_bzero(arr, (nmemb * size));
	return (arr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:56:10 by aezghari          #+#    #+#             */
/*   Updated: 2024/10/28 20:39:26 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbrlen(int nbr)
{
	int	count;

	count = 0;
	if (nbr == 0)
		return (1);
	while (nbr)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}

static char	*itoav2(int n)
{
	char	*str;
	int		len;
	int		isneg;

	isneg = 0;
	if (n < 0)
	{
		n = -n;
		isneg = 1;
	}
	len = nbrlen(n) + isneg;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len--] = '\0';
	if (n == 0)
		str[len--] = '0';
	while (n)
	{
		str[len--] = n % 10 + '0';
		n /= 10;
	}
	if (isneg)
		str[len] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else
		return (itoav2(n));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:54:16 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/26 18:38:40 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*allocate_del(char *del, int len, t_shell *shell)
{
	char	*str;
	int		i;
	int		j;

	j = 0;
	i = 0;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	add_to_allocator(str, &shell->gc);
	while (del[i])
	{
		if (del[i] == '"' || del[i] == '\'')
		{
			i++;
			continue ;
		}
		str[j] = del[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}

static int	get_del_len(char *del)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (del[i])
	{
		if (del[i] == '"' || del[i] == '\'')
		{
			i++;
			continue ;
		}
		len++;
		i++;
	}
	return (len);
}

char	*quote_remover(char *del, int *expand, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	*expand = 1;
	while (del[i])
	{
		if (del[i] == '"' || del[i] == '\'')
			*expand = 0;
		i++;
	}
	len = get_del_len(del);
	return (allocate_del(del, len, shell));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:20:19 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/26 18:23:28 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_sorted_env(t_env *env, t_shell *shell)
{
	t_env	*sorted;

	sorted = copy_env_list(env, shell);
	sort_env_list(sorted);
	while (sorted)
	{
		printf("declare -x %s", sorted->key);
		if (sorted->value)
			printf("=\"%s\"", sorted->value);
		printf("\n");
		sorted = sorted->next;
	}
}

int	check_identifier(char *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	if (!ft_isalpha(cmd[0]) && cmd[0] != '_')
		return (custom_err("not a valid identifier", cmd, 1, shell), 1);
	while (cmd[i] && cmd[i] != '=')
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (custom_err("not a valid identifier", cmd, 1, shell), 1);
		i++;
	}
	return (0);
}

char	*get_identifier(char *str, t_shell *shell)
{
	int		len;
	char	*str_2;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	str_2 = ft_substr(str, 0, len);
	if (!str_2)
		allocation_failure(shell);
	return (str_2);
}

void	sort_env_list(t_env *env)
{
	char	*tmp_key;
	char	*tmp_value;
	t_env	*cur;

	while (env)
	{
		cur = env->next;
		while (cur)
		{
			if (ft_strcmp(env->key, cur->key) > 0)
			{
				tmp_key = env->key;
				tmp_value = env->value;
				env->key = cur->key;
				env->value = cur->value;
				cur->key = tmp_key;
				cur->value = tmp_value;
			}
			cur = cur->next;
		}
		env = env->next;
	}
}

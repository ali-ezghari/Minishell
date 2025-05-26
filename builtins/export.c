/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:24:08 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/26 18:19:36 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	add_env_back(t_env **env_list, t_env *new_env)
{
	t_env	*tmp;

	if (!env_list || !new_env)
		return ;
	if (!*env_list)
	{
		*env_list = new_env;
		return ;
	}
	tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_env;
}

static t_env	*copy_env_list(t_env *env, t_shell *shell)
{
	t_env	*copy;
	t_env	*new_node;

	copy = NULL;
	while (env)
	{
		new_node = ft_malloc(sizeof(t_env), &shell->gc);
		new_node->key = ft_strdup(env->key);
		new_node->value = ft_strdup(env->value);
		add_to_allocator(new_node->key, &shell->gc);
		add_to_allocator(new_node->value, &shell->gc);
		new_node->next = NULL;
		add_env_back(&copy, new_node);
		env = env->next;
	}
	return (copy);
}

static void	update_env(t_env **envp, t_env *env, t_shell *shell)
{
	t_env	*head;

	head = *envp;
	if (!envp || !env)
		return ;
	while (head)
	{
		if (!ft_strcmp(env->key, head->key))
		{
			if (!env->value)
				return ;
			head->value = ft_strdup(env->value);
			add_to_allocator(head->value, &shell->gc);
			return ;
		}
		head = head->next;
	}
	add_env_back(envp, env);
}

void	bin_export(t_command *cmd, t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	if (!cmd->av[1])
		return (print_sorted_env(shell->envp, shell));
	while (cmd->av[++i])
	{
		if (check_identifier(cmd->av[i], shell))
			return ;
		key = get_identifier(cmd->av[i], shell);
		value = NULL;
		if (ft_strchr(cmd->av[i], '='))
			value = ft_strdup(ft_strchr(cmd->av[i], '=') + 1);
		update_env(&shell->envp, new_env(key, value, shell), shell);
		if (key)
			free(key);
		if (value)
			free(value);
	}
	shell->exit_status = 0;
}

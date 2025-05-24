/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:24:08 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:24:33 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env *new_env(char *key, char *value, t_shell *shell)
{
    t_env *node;

    node = ft_malloc(sizeof(t_env), &shell->gc);
    if (!node)
        return (NULL);
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    add_to_allocator(node->key, &shell->gc);
    add_to_allocator(node->value, &shell->gc);
    node->next = NULL;
    return (node);
}

void add_env_back(t_env **env_list, t_env *new_env)
{
    t_env *tmp;

    if (!env_list || !new_env)
        return;
    if (!*env_list)
    {
        *env_list = new_env;
        return;
    }
    tmp = *env_list;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_env;
}

t_env *copy_env_list(t_env *env, t_shell *shell)
{
    t_env *copy = NULL;
    t_env *new_node;

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

void sort_env_list(t_env *env)
{
    char *tmp_key;
    char *tmp_value;
    t_env *cur;

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

void update_env(t_env **envp, t_env *env, t_shell *shell)
{
    t_env *head;

    head = *envp;
    if (!envp || !env)
        return;
    while (head)
    {
        if (!ft_strcmp(env->key, head->key))
        {

            if (!env->value)
            {
            }

            head->value = ft_strdup(env->value); // if the var is already in the env list it will be updated
            add_to_allocator(head->value, &shell->gc);
            return;
        }
        head = head->next;
    }
    add_env_back(envp, env); // if the var is new it will be inserted into the env list
}

void print_sorted_env(t_env *env, t_shell *shell)
{
    t_env *sorted;

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

int check_identifier(char *cmd, t_shell *shell)
{
    int i;

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
char *get_identifier(char *str, t_shell *shell)
{
    int len;
    char *str_2;

    len = 0;
    while (str[len] && str[len] != '=')
        len++;
    str_2 = ft_substr(str, 0, len);
    if (!str_2)
        allocation_failure(shell);
    return (str_2);
}

void bin_export(t_command *cmd, t_shell *shell)
{
    int i;
    char *key;
    char *value;

    i = 0;
    if (!cmd->av[1])
        return (print_sorted_env(shell->envp, shell));
    while (cmd->av[++i])
    {
        if (check_identifier(cmd->av[i], shell))
            return;
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

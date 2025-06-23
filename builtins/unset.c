/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:22:52 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:23:51 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void unset_one_var(char *var, t_shell *shell)
{
    t_env *prev;
    t_env *curr;
	
    curr = shell->envp;
	prev = NULL;
    while (curr)
    {
        if (!ft_strcmp(var, curr->key))
        {
            if (prev)
                prev->next = curr->next;
            else
                shell->envp = curr->next;
            curr = curr->next;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void	bin_unset(t_command *cmd, t_shell *shell)
{
	int	i;

	i = 1;
	while (cmd->av[i])
	{
		unset_one_var(cmd->av[i], shell);
		i++;
	}
	shell->exit_status = 0;
}

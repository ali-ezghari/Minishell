/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:17:34 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:17:35 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	bin_env(t_command *cmd, t_shell *shell)
{
	t_env	*env;

	env = shell->envp;
	if (cmd->av[1])
	{
		printf("Minishell: env: too many arguments\n");
		shell->exit_status = 1;
		return ;
	}
	while (env)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	shell->exit_status = 0;
}

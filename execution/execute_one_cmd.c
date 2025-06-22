/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_one_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:45:17 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/23 09:47:20 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	child_pr(t_command *cmd, t_shell *shell)
{
	char	*full_cmd;

	if (!cmd->av || !cmd->av[0])
		exit(0);
	full_cmd = get_path1(cmd->av[0], shell);
	if (!full_cmd)
		exit(shell->exit_status);
	execve(full_cmd, cmd->av, shell->envs);
	perror("execve");
	exit(127);
}

void	execute_one_cmd(t_command *cmd, t_shell *shell)
{
	pid_t	child;
	int		status;

	if (open_files(cmd->redirs, shell))
	{
		shell->exit_status = 1;
		return ;
	}
	if (execute_builtin(cmd, shell))
		return ;
	child = fork();
	if (child == -1)
		return (perror("fork error"));
	if (child == 0)
		child_pr(cmd, shell);
	waitpid(child, &status, 0);
	get_exit_code(&status, shell);
}

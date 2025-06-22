/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:27:50 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 15:30:03 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_exit_code(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

int	execute_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd->av || !cmd->av[0])
		return (0);
	else if (!strcmp(cmd->av[0], "echo"))
		return (bin_echo(cmd, shell), 1);
	else if (!strcmp(cmd->av[0], "cd"))
		return (bin_cd(cmd, shell), 1);
	else if (!strcmp(cmd->av[0], "export"))
		return (bin_export(cmd, shell), 1);
	else if (!strcmp(cmd->av[0], "unset"))
		return (bin_unset(cmd, shell), 1);
	else if (!strcmp(cmd->av[0], "pwd"))
		return (bin_pwd(shell), 1);
	else if (!strcmp(cmd->av[0], "env"))
		return (bin_env(cmd, shell), 1);
	else if (!strcmp(cmd->av[0], "exit"))
		return (bin_exit(cmd, shell), 1);
	return (0);
}

void	execution(t_shell *shell)
{
	t_command *cmd = shell->cmds;
	pid_t pid;
	int status;

	if (!cmd || !cmd->av || !cmd->av[0])
		return;

	// Handle built-in commands
	if (execute_builtin(cmd, shell))
		return;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		char *path = get_path1(cmd->av[0], shell);
		if (!path)
		{
			fprintf(stderr, "minishell: %s: command not found\n", cmd->av[0]);
			exit(127);
		}
		execve(path, cmd->av, shell->envs);
		perror("minishell");
		exit(126);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		shell->exit_status = WEXITSTATUS(status);
	}
	else
	{
		perror("minishell");
		shell->exit_status = 1;
	}
}

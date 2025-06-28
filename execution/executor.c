/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:27:50 by aezghari          #+#    #+#             */
/*   Updated: 2025/06/26 09:35:54 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_exit_code(int *status, t_shell *shell)
{
	if (WIFEXITED(*status))
		shell->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		shell->exit_status = 128 + WTERMSIG(*status);
		if (WTERMSIG(*status) == SIGINT || WTERMSIG(*status) == SIGQUIT)
			write(1, "\n", 1);
	}
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
	t_command	*cmd;
	int			count;

	cmd = shell->cmds;
	shell->is_forked = 0;
	count = 0;
	in_out_backup(shell);
	if (handle_heredoc(cmd, shell))
		return ;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	if (count == 1)
		execute_one_cmd(shell->cmds, shell);
	else if (count > 1)
		execute_multiple_cmds(count, shell->cmds, shell);
	close_files(shell->cmds->redirs);
	restore_fds(shell->out_fd_b, shell->in_fd_b);
}

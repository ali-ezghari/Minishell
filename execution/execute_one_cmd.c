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

	signal(SIGQUIT, SIG_DFL);
	if (!cmd->av || !cmd->av[0])
		exit(0);
	full_cmd = get_path1(cmd->av[0], shell);
	if (!full_cmd)
		exit(shell->exit_status);
	execve(full_cmd, cmd->av, env_list_to_array(shell->envp, shell));
	perror("execve");
	close_files(cmd->redirs);
	restore_fds(shell->in_fd_b, shell->out_fd_b);
	ft_lstclear(&gc, free);
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
	signal(SIGINT, SIG_IGN);
	waitpid(child, &status, 0);
	sig_setup();
	get_exit_code(&status, shell);
}

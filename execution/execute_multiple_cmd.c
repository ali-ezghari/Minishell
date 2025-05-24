/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_multiple_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 09:47:35 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/23 09:52:19 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	wait_for_children(pid_t *pids, int cmd_count, t_shell *shell)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
		waitpid(pids[i++], &status, 0);
	get_exit_code(status, shell);
}

static void	exec_child(t_command *cmd, t_shell *shell)
{
	char	*full_cmd;

	if (!cmd->av || !cmd->av[0])
		exit(0);
	if (open_files(cmd->redirs, shell))
		exit(1);
	full_cmd = get_path1(cmd->av[0], shell);
	if (!full_cmd)
		allocation_failure(shell);
	if (execute_builtin(cmd, shell))
		exit(shell->exit_status);
	execve(full_cmd, cmd->av, shell->envs);
	perror("execve");
	exit(1);
}

int	init_pids_fds(int cmd_count, pid_t **pids, int **fds, t_shell *shell)
{
	int	i;

	i = 0;
	*pids = ft_malloc(sizeof(pid_t) * cmd_count, &shell->gc);
	*fds = ft_malloc(sizeof(int) * 2 * (cmd_count - 1), &shell->gc);
	if (!*pids || !*fds)
		return (allocation_failure(shell), 1);
	while (i < cmd_count - 1)
	{
		if (pipe(*fds + i * 2) == -1)
			return (custom_err("pipe", NULL, 1, shell), 1);
		i++;
	}
	return (0);
}

static void	child_pipes_setup(int *fds, int count, int i)
{
	if (i > 0)
		dup_in(fds[(i - 1) * 2]);
	if (i < count - 1)
		dup_out(fds[i * 2 + 1]);
	clean_up_fds(2 * (count - 1), fds);
}

void	execute_multiple_cmds(int count, t_command *cmd, t_shell *shell)
{
	pid_t	*pids;
	int		*fds;
	int		i;

	if (init_pids_fds(count, &pids, &fds, shell) == 1)
		return ;
	i = 0;
	while (cmd && i < count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			shell->is_forked = 1;
			child_pipes_setup(fds, count, i);
			exec_child(cmd, shell);
		}
		i++;
		cmd = cmd->next;
	}
	clean_up_fds(2 * (count - 1), fds);
	wait_for_children(pids, count, shell);
}

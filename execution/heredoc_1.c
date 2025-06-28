/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:32:14 by aezghari          #+#    #+#             */
/*   Updated: 2025/06/26 09:40:50 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	heredoc_child(char *del, int fd, int expand, t_shell *shell)
{
	char	*rl;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		rl = readline("> ");
		if (rl)
			add_to_allocator(rl);
		if (!rl || !ft_strcmp(rl, del))
			break ;
		if (expand == 1 && ft_strchr(rl, '$'))
			rl = handle_exp(rl, shell, shell->exit_status);
		write(fd, rl, ft_strlen(rl));
		write(fd, "\n", 1);
	}
	close(fd);
}

static int	fill_heredoc(t_shell *shell, t_redir *redir)
{
	int		fd[2];
	char	*del;
	int		expand;
	pid_t	child;
	int		status;

	del = quote_remover(redir->filename, &expand, shell);
	if (pipe(fd) == -1)
		return (pipe_err(shell), 1);
	signal(SIGINT, SIG_IGN);
	child = fork();
	if (child == 0)
	{
		close(fd[0]);
		heredoc_child(del, fd[1], expand, shell);
		exit(0);
	}
	waitpid(child, &status, 0);
	close(fd[1]);
	sig_setup();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (shell->exit_status = 130, close(fd[0]), write(1, "\n", 1), 1);
	redir->fd = fd[0];
	get_exit_code(&status, shell);
	return (0);
}

int	handle_heredoc(t_command *cmd, t_shell *shell)
{
	t_redir	*redir;
	int		st;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == T_HEREDOC)
			{
				st = fill_heredoc(shell, redir);
				if (st != 0)
				{
					shell->exit_status = st;
					return (1);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:32:14 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 15:35:52 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	fill_heredoc(t_shell *shell, t_redir *redir)
{
	int		fd[2];
	char	*rl;
	char	*del;
	int		expand;

	del = quote_remover(redir->filename, &expand, shell);
	if (pipe(fd) == -1)
		return (pipe_err(shell), 1);
	while (1)
	{
		rl = readline("> ");
		if (rl)
			add_to_allocator(rl, &shell->gc);
		if (!rl || !ft_strcmp(rl, del))
			break ;
		if (expand == 1 && ft_strchr(rl, '$'))
			rl = handle_exp(rl, shell, shell->exit_status);
		write(fd[1], rl, ft_strlen(rl));
		write(fd[1], "\n", 1);
	}
	close(fd[1]);
	redir->fd = fd[0];
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

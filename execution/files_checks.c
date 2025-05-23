/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:30:21 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 15:32:03 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_files(t_redir *redir)
{
	while (redir)
	{
		if (redir->fd >= 0)
			close(redir->fd);
		redir = redir->next;
	}
}

static int	open_nd_redir(t_redir *redir, t_shell *shell)
{
	int	fd;

	if (redir->type == T_REDIRECT_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == T_REDIRECT_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == T_APPEND)
		fd = open(redir->filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else if (redir->type == T_HEREDOC)
		fd = dup(redir->fd);
	if (fd == -1)
		return (custom_cmd_err(strerror(errno), redir->filename, 1, shell), 1);
	redir->fd = fd;
	if (redir->type == T_REDIRECT_IN || redir->type == T_HEREDOC)
		dup_in(fd);
	else
		dup_out(fd);
	return (0);
}

int	open_files(t_redir *redir, t_shell *shell)
{
	while (redir)
	{
		if (open_nd_redir(redir, shell))
			return (1);
		redir = redir->next;
	}
	return (0);
}

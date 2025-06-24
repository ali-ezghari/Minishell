/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:17:45 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:19:29 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_exit_err(char *msg, char *name, int status, t_shell *shell)
{
	write(2, "Minishell: exit: ", 17);
	if (name)
	{
		write(2, name, strlen(name));
		write(2, ": ", 2);
	}
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
	shell->exit_status = status;
}

static int	is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	if (s[0] == '-' || s[0] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	if (i >= 20 && s[0] != '0')
		return (2);
	return (1);
}

static int	check_errors(t_command *cmd, t_shell *shell)
{
	if (is_numeric(cmd->av[1]) && cmd->av[2])
	{
		print_exit_err("too many arguments", NULL, 1, shell);
		return (2);
	}
	else if (!is_numeric(cmd->av[1]) || is_numeric(cmd->av[1]) == 2)
	{
		print_exit_err("numeric argument required", cmd->av[1], 2, shell);
		return (2);
	}
	else
		return (ft_atoi(cmd->av[1]) % 256);
}

void	bin_exit(t_command *cmd, t_shell *shell)
{
	int	st;

	st = 0;
	if (!cmd->av[1])
		st = shell->exit_status;
	else
		st = check_errors(cmd, shell);
	if (shell->is_forked == 0)
		write(1, "exit\n", 5); 
	close_files(cmd->redirs);
	restore_fds(shell->in_fd_b, shell->out_fd_b);
	ft_lstclear(&gc, free);
	exit(st);
}

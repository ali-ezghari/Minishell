/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezghari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:15:53 by aezghari          #+#    #+#             */
/*   Updated: 2025/05/22 11:16:45 by aezghari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_cd_err(char *msg, char *name, int status, t_shell *shell)
{
	write(2, "Minishell: cd: ", 15);
	if (name)
	{
		write(2, name, strlen(name));
		write(2, ": ", 2);
	}
	write(2, msg, strlen(msg));
	write(2, "\n", 1);
	shell->exit_status = status;
}

static void	cd_home(t_shell *shell)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
		print_cd_err("HOME not set", NULL, 1, shell);
	else if (chdir(path) == -1)
		print_cd_err(strerror(errno), path, 1, shell);
	else
		shell->exit_status = 0;
}

void	bin_cd(t_command *cmd, t_shell *shell)
{
	if (!cmd->av[1])
		cd_home(shell);
	else if (cmd->av[2])
		print_cd_err("Too many arguments", NULL, 1, shell);
	else if (chdir(cmd->av[1]) == -1)
		print_cd_err(strerror(errno), cmd->av[1], 1, shell);
	else
		shell->exit_status = 0;
}

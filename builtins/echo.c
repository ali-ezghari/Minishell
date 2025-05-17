#include "../includes/minishell.h"

void bin_echo(t_command *cmd, t_shell *shell)
{
	int i;
	int flag;

	flag = 1;
	i = 1;
	while (cmd->av[i] && !ft_strcmp(cmd->av[i], "-n"))
	{
		i++;
		flag = 0;
	}
	while (cmd->av[i])
	{
		printf("%s", cmd->av[i]);
		if (cmd->av[i + 1])
			printf(" ");
		i++;
	}
	if (flag)
		printf("\n");
	shell->exit_status = 0;
}
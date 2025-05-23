

#include "../includes/minishell.h"



void	handle_exp(char **inp, t_shell *shell, int exit_status)
{
	if (!inp || !*inp)
		return ;
	ft_expand_exit_status(inp, exit_status);
	ft_expand_vars(inp, shell);
}

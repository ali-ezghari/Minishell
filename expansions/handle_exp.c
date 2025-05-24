

#include "../includes/minishell.h"

char	*handle_exp(char *inp, t_shell *shell, int exit_status)
{
	char	*result;
	char	*tmp;

	if (!inp || !shell || !shell->envp)
		return (NULL);
	result = ft_expand_exit_status(inp, exit_status);
	if (!result)
		return (NULL);
	tmp = ft_expand_vars(result, shell);
	free(result);
	return (tmp);
}

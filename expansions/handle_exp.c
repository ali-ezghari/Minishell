#include "../includes/minishell.h"

char	*handle_exp(char *inp, t_shell *shell, int exit_status)
{
	char	*result;
	char	*tmp;

	if (!inp || !shell || !shell->envp || !shell->gc)
		return (NULL);
	result = ft_expand_exit_status(inp, exit_status, &shell->gc);
	if (!result)
		return (NULL);
	tmp = ft_expand_vars(result, shell);
	if (!tmp)
	{
		free(result);
		return (NULL);
	}
	if (tmp != result)
		free(result);
	return (tmp);
}

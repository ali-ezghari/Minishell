
#include "../includes/minishell.h"


int	is_var_name(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	is_valid_var_name(char *name)
{
	if (!name || !*name || !ft_isalpha(*name))
		return (FALSE);
	while (*name)
	{
		if (!is_var_name(*name))
			return (FALSE);
		name++;
	}
	return (TRUE);
}

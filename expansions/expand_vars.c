
#include "../includes/minishell.h"


static char	*var_pos(char *str)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == '\'')
		{
			str++;
			while (*str && *str != '\'')
				str++;
		}
		if (*str == '\"')
		{
			str++;
			while (*str && *str != '\"')
			{
				if (*str == '$' && is_var_name(str[1]))
					return (str);
				str++;
			}
		}
		if (*str == '$' && is_var_name(str[1]))
			return (str);
		str++;
	}
	return (NULL);
}

static void	update_inp(char **inp, char *var_value, char *scnd_part)
{
	char	*fst_part;
	char	*update_inp;

	if (!inp || !*inp)
		return ;
	if (!*inp[0] && !var_value)
		fst_part = ft_strdup("");
	else if (!*inp[0] && var_value)
		fst_part = ft_strdup(var_value);
	else if (!var_value)
		fst_part = ft_strdup(*inp);
	else
		fst_part = ft_strjoin(*inp, var_value);
	if (!fst_part)
		return ;
	update_inp = ft_strjoin(fst_part, scnd_part);
	free(fst_part);
	free(*inp);
	*inp = update_inp;
}

void	ft_expand_vars(char **inp, t_shell *shell)
{
	char	*var_pos;
	char	*var_value;
	char	*var_name;
	int		var_size;

	if (!inp || !*inp || !shell)
		return ;
	var_pos = var_pos(*inp);
	if (var_pos)
	{
		var_size = 0;
		while (is_var_name(var_pos[var_size + 1]))
			var_size++;
		var_name = ft_substr(var_pos, 1, var_size);
		if (!var_name)
			return ;
		*var_pos = '\0';
		var_value = ft_getenv(var_name, shell->envp);
		update_inp(inp, var_value, (var_pos + var_size + 1));
		free(var_name);
		ft_expand_vars(inp, shell);
	}
}

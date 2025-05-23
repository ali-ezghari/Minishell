

#include "../includes/minishell.h"


static char	*exit_status_pos(char *str)
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
				if (*str == '$' && str[1] == '?')
					return (str);
				str++;
			}
		}
		if (*str == '$' && str[1] == '?')
			return (str);
		str++;
	}
	return (NULL);
}

static void	update_inp(char **inp, char *exit_code, char *scnd_part)
{
	char	*fst_part;
	char	*update_inp;

	if (!inp || !*inp || !exit_code)
		return ;
	fst_part = ft_strjoin(*inp, exit_code);
	if (!fst_part)
		return ;
	update_inp = ft_strjoin(fst_part, scnd_part);
	free(*inp);
	free(fst_part);
	*inp = update_inp;
}

void	ft_expand_exit_status(char **inp, int exit_status)
{
	char	*exit_status_pos;
	char	*exit_status_str;

	if (!inp || !*inp)
		return ;
	exit_status_pos = exit_status_pos(*inp);
	if (exit_status_pos)
	{
		*exit_status_pos = '\0';
		exit_status_str = ft_itoa(exit_status);
		if (!exit_status_str)
			return ;
		update_inp(inp, exit_status_str, (exit_status_pos + 2));
		free(exit_status_str);
		ft_expand_exit_status(inp, exit_status);
	}
}



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

static char	*update_str(char *str, char *exit_code, char *scnd_part)
{
	char	*fst_part;
	char	*result;

	if (!str || !exit_code)
		return (NULL);
	fst_part = ft_strjoin(str, exit_code);
	if (!fst_part)
		return (NULL);
	result = ft_strjoin(fst_part, scnd_part);
	free(fst_part);
	return (result);
}

char	*ft_expand_exit_status(char *str, int exit_status)
{
	char	*pos;
	char	*exit_status_str;
	char	*result;
	char	*tmp;

	if (!str)
		return (NULL);
	pos = exit_status_pos(str);
	if (!pos)
		return (ft_strdup(str));
	tmp = ft_strndup(str, pos - str);
	if (!tmp)
		return (NULL);
	exit_status_str = ft_itoa(exit_status);
	if (!exit_status_str)
	{
		free(tmp);
		return (NULL);
	}
	result = update_str(tmp, exit_status_str, pos + 2);
	free(tmp);
	free(exit_status_str);
	if (!result)
		return (NULL);
	tmp = ft_expand_exit_status(result, exit_status);
	free(result);
	return (tmp);
}

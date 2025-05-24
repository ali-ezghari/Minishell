
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

static char	*get_env_value(char *var_name, t_env *envp)
{
	t_env	*current;
	int		len;

	if (!var_name || !envp)
		return (NULL);
	len = ft_strlen(var_name);
	current = envp;
	while (current)
	{
		if (ft_strncmp(current->name, var_name, len) == 0 && current->name[len] == '\0')
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*update_str(char *str, char *var_value, char *scnd_part)
{
	char	*fst_part;
	char	*result;

	if (!str)
		return (NULL);
	if (!str[0] && !var_value)
		fst_part = ft_strdup("");
	else if (!str[0] && var_value)
		fst_part = ft_strdup(var_value);
	else if (!var_value)
		fst_part = ft_strdup(str);
	else
		fst_part = ft_strjoin(str, var_value);
	if (!fst_part)
		return (NULL);
	result = ft_strjoin(fst_part, scnd_part);
	free(fst_part);
	return (result);
}

char	*ft_expand_vars(char *str, t_shell *shell)
{
	char	*pos;
	char	*var_value;
	char	*var_name;
	char	*result;
	char	*tmp;
	int		var_size;

	if (!str || !shell || !shell->envp)
		return (NULL);
	pos = var_pos(str);
	if (!pos)
		return (ft_strdup(str));
	var_size = 0;
	while (is_var_name(pos[var_size + 1]))
		var_size++;
	var_name = ft_substr(pos, 1, var_size);
	if (!var_name)
		return (NULL);
	tmp = ft_strndup(str, pos - str);
	if (!tmp)
	{
		free(var_name);
		return (NULL);
	}
	var_value = get_env_value(var_name, shell->envp);
	result = update_str(tmp, var_value, pos + var_size + 1);
	free(tmp);
	free(var_name);
	if (!result)
		return (NULL);
	tmp = ft_expand_vars(result, shell);
	free(result);
	return (tmp);
}

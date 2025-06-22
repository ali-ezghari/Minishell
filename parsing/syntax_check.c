#include "../includes/minishell.h"
#include "../libft/libft.h"

static int	is_redirection_type(t_token_type type)
{
	return (type == T_REDIRECT_IN || type == T_REDIRECT_OUT
		|| type == T_APPEND || type == T_HEREDOC);
}

static int	check_pipe(t_token *tok, t_shell *shell)
{
	// Skip if the token is quoted (single or double quotes)
	if (tok->quoted)
		return (1);
	if (tok->type != T_PIPE)
		return (1);
	if (!tok->next || tok->next->type == T_PIPE)
	{
		print_synerror("|");
		shell->exit_status = 2;
		return (0);
	}
	return (1);
}

static int	check_redirection(t_token *tok, t_shell *shell)
{
	// Skip if the token is quoted (single or double quotes)
	if (tok->quoted)
		return (1);
	if (!is_redirection_type(tok->type))
		return (1);
	if (!tok->next || tok->next->type != T_WORD)
	{
		print_synerror("newline");
		shell->exit_status = 2;
		return (0);
	}
	return (1);
}

int	catch_syntax_errors(t_token *tok, t_shell *shell)
{
	if (!tok || !shell)
		return (0);
	// Check for pipe at start
	if (tok->type == T_PIPE && !tok->quoted)
	{
		print_synerror("|");
		shell->exit_status = 2;
		return (1);
	}
	while (tok)
	{
		// Skip quoted tokens for syntax checking
		if (!tok->quoted)
		{
			if (!check_pipe(tok, shell) || !check_redirection(tok, shell))
				return (1);
		}
		tok = tok->next;
	}
	return (0);
}

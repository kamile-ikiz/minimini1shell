/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/01 16:44:11 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirect_token(token_type_t type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC);
}

static int	handle_redirect_pair(token_type_t redirect_type, const char *filename,
	command_t *cmd)
{
	redirect_t	*new_redirect;

	new_redirect = create_redirect(redirect_type, filename);
	if (!new_redirect)
		return (-1);
	add_redirect(&cmd->redirects, new_redirect);
	return (0);
}

/*
** Example parsing function
** Usage: parse tokens, handle redirects first, then add remaining words to cmd
*/
int	parse_command_example(token_type_t *tokens, char **words, int token_count,
	command_t *cmd)
{
	int	i;

	i = 0;
	while (i < token_count)
	{
		if (is_redirect_token(tokens[i]))
		{
			if (i + 1 >= token_count || tokens[i + 1] != TOKEN_WORD)
				return (-1);
			if (handle_redirect_pair(tokens[i], words[i + 1], cmd) != 0)
				return (-1);
			i += 2;
		}
		else if (tokens[i] == TOKEN_WORD)
		{
			if (add_word_to_command(cmd, words[i]) != 0)
				return (-1);
			i++;
		}
		else
			i++;
	}
	return (0);
}
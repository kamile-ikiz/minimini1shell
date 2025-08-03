/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/02 16:55:50 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Example parsing function
** Usage: parse tokens, handle redirects first, then add remaining words to cmd
*/
int	parse_command_or_redirect(token_t **tokens, command_t *cmd)
{
	int	i;
	int token_count;

	token_count = get_token_count(*tokens);
	i = 0;
	while (i < token_count)
	{
		if (is_redirect_token(*tokens[i]))
		{
			if (i + 1 >= token_count || tokens[i + 1] != TOKEN_WORD)
				return (-1);
			if (handle_redirect_pair(tokens[i], tokens[i + 1], cmd) != 0)
				return (-1);
			i += 2;
		}
		else if (tokens[i] == TOKEN_WORD)
		{
			if (handle_command_pair(tokens[i], cmd) != 0)
				return (-1);
			i++;
		}
		else
			i++;
	}
	return (0);
}

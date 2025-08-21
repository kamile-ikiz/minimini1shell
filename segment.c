/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 04:09:05 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_redirect(t_token *redirect_token, t_command *cmd)
{
	if (!redirect_token->next || redirect_token->next->type != TOKEN_WORD)
	{
		set_exit_code(2);
		return (-1);
	}
	if (handle_redirect_pair(redirect_token, redirect_token->next, cmd) != 0)
	{
		set_exit_code(1);
		return (-1);
	}
	return (0);
}

static int	process_word(t_token *word_token, t_command *cmd)
{
	if (handle_command_pair(word_token, cmd) != 0)
	{
		set_exit_code(1);
		return (-1);
	}
	return (0);
}

static int	process_and_advance(t_token **current_ptr, t_command *cmd)
{
	t_token	*current;

	current = *current_ptr;
	if (is_redirect_token(*current))
	{
		if (process_redirect(current, cmd) == -1)
			return (-1);
		*current_ptr = current->next->next;
	}
	else if (current->type == TOKEN_WORD)
	{
		if (process_word(current, cmd) == -1)
			return (-1);
		*current_ptr = current->next;
	}
	else
	{
		*current_ptr = current->next;
	}
	return (0);
}

int	parse_command_or_redirect(t_segment *segment, t_command **cmd_ptr)
{
	t_token	*current;

	if (!segment || !cmd_ptr)
		return (set_exit_code(1), -1);
	*cmd_ptr = create_command();
	if (!*cmd_ptr)
		return (set_exit_code(1), -1);
	current = segment->tokens;
	while (current)
	{
		if (process_and_advance(&current, *cmd_ptr) == -1)
			return (parse_error(cmd_ptr));
	}
	return (0);
}

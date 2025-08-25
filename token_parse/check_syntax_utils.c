/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:12:10 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 15:21:17 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_token_at_index(t_token *head, int index)
{
	int		i;
	t_token	*current;

	if (!head || index < 0)
		return (NULL);
	current = head;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	return (current);
}

int	get_token_count(t_token *head)
{
	int		count;
	t_token	*current;

	if (!head)
		return (0);
	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static int	check_pipe_start_end(t_token *head, int token_count)
{
	t_token	*last;

	if (token_count == 0)
		return (1);
	if (head->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	last = get_token_at_index(head, token_count - 1);
	if (last && last->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
		{
			print_syntax_error("|");
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

int	check_pipe_syntax(t_token *tokens, int token_count)
{
	if (!check_pipe_start_end(tokens, token_count))
	{
		return (0);
	}
	if (!check_consecutive_pipes(tokens))
		return (0);
	tokens = tokens->next;
	return (1);
}

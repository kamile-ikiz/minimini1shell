/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:17 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 04:09:12 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens_until_pipe(t_token *token_list)
{
	int		count;
	t_token	*current;

	count = 0;
	current = token_list;
	while (current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	add_segment(t_segment **head, t_segment *new_segment)
{
	t_segment	*current;

	if (!*head)
	{
		*head = new_segment;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_segment;
}

t_token	*skip_to_next_segment(t_token *current)
{
	while (current && current->type != TOKEN_PIPE)
		current = current->next;
	if (current && current->type == TOKEN_PIPE)
		current = current->next;
	return (current);
}

static int	process_one_segment(t_token **current_ptr, t_segment **segments_ptr)
{
	int			seg_token_count;
	t_segment	*new_segment;

	seg_token_count = count_tokens_until_pipe(*current_ptr);
	if (seg_token_count == 0)
	{
		*current_ptr = (*current_ptr)->next;
		return (0);
	}
	new_segment = create_single_segment(*current_ptr, seg_token_count);
	if (!new_segment)
	{
		set_exit_code(1);
		return (-1);
	}
	add_segment(segments_ptr, new_segment);
	*current_ptr = skip_to_next_segment(*current_ptr);
	return (0);
}

t_segment	*split_tokens_by_pipe(t_token *token_list)
{
	t_segment	*segments;
	t_token		*current;

	segments = NULL;
	current = token_list;
	while (current)
	{
		if (process_one_segment(&current, &segments) == -1)
		{
			free_segments(segments);
			return (NULL);
		}
	}
	return (segments);
}

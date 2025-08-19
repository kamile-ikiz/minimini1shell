/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 15:39:15 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *skip_to_next_segment(t_token *current)
{
	while(current && current->type != TOKEN_PIPE)
		current = current->next;
	if(current && current->type == TOKEN_PIPE)
		current = current->next;
	return(current);
}

void	add_segment(t_segment **head, t_segment *new_segment)
{
	t_segment *current;

	if(!*head)
	{
		*head = new_segment;
		return;
	}
	current = *head;
	while(current->next)
		current = current->next;
	current->next = new_segment;
}

t_segment	*split_tokens_by_pipe(t_token *token_list)
{
	t_segment	*segments;
	t_segment	*new_segment;
	t_token		*current;
	int			seg_token_count;

	segments = NULL;
	current = token_list;
	while(current)
	{
		seg_token_count = count_tokens_until_pipe(current);
		if(seg_token_count == 0)
		{
			current = current->next;
			continue;
		}
		new_segment = create_single_segment(current, seg_token_count);
		if(!new_segment)
		{
			free_segments(segments);
			set_exit_code(1);
			return(NULL);
		}
		add_segment(&segments, new_segment);
		current = skip_to_next_segment(current);
	}
	return(segments);	
}

static void advance_to_next_token(t_token **current)
{
	if (!current || !*current)
		return;
	if (is_redirect_token(**current))
	{
		*current = (*current)->next;
		if (*current)
			*current = (*current)->next;
	}
	else
		*current = (*current)->next;
}

int	parse_command_or_redirect(t_segment *segment, t_command **cmd_ptr)
{
	t_token *current;

	if(!segment || !cmd_ptr)
	{
		set_exit_code(1);
		return (-1);
	}
	*cmd_ptr = create_command();
	if (!*cmd_ptr)
	{
		set_exit_code(1);
		return (-1);
	}
	current = segment->tokens;
	while(current)
	{
		if (is_redirect_token(*current))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				set_exit_code(2);
				return (parse_error(cmd_ptr));
			}	
			if (handle_redirect_pair(current, current->next, *cmd_ptr) != 0)
			{
				set_exit_code(1);
				return (parse_error(cmd_ptr));
			}
		}
		else if (current->type == TOKEN_WORD)
		{
			if (handle_command_pair(current, *cmd_ptr) != 0)
			{
				set_exit_code(1);
				return (parse_error(cmd_ptr));
			}
		}
		advance_to_next_token(&current);
	}
	return (0);
}

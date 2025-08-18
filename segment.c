/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/18 19:38:42 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_t *skip_to_next_segment(token_t *current)
{
	while(current && current->type != TOKEN_PIPE)
		current = current->next;
	if(current && current->type == TOKEN_PIPE)
		current = current->next;
	return(current);
}

void	add_segment(segment_t **head, segment_t *new_segment)
{
	segment_t *current;

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

segment_t	*split_tokens_by_pipe(token_t *token_list)
{
	segment_t	*segments;
	segment_t	*new_segment;
	token_t		*current;
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

static void advance_to_next_token(token_t **current)
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

int	parse_command_or_redirect(segment_t *segment, command_t **cmd_ptr)
{
	token_t *current;

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

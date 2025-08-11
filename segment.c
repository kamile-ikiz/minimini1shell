/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/11 16:48:30 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_tokens_until_pipe(token_t *token_list)
{
	int count;
	token_t *current;

	count = 0;
	current = token_list;
	while(current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}
	return (count);
}

segment_t *create_segment(void)
{
	segment_t	*segment;

	segment = malloc(sizeof(segment_t));
	if (!segment)
		return (NULL);
	segment->tokens = NULL;
	segment->token_count = 0;
	segment->next = NULL;
	return (segment);
}

token_t	*create_token_copy(token_t *src)
{
	token_t	*new;
	new = malloc(sizeof(token_t));
	if(!new)
		return (NULL);
	new->type = src->type;
	new->value = ft_strdup(src->value);
	new->next = NULL;
	return (new);
}

token_t *copy_tokens_until_pipe(token_t *start_token, int count)
{
	token_t *new_list;
	token_t *new_token;
	token_t *current;
	token_t *last;
	int i;

	new_list = NULL;
	last = NULL;
	current = start_token;
	i = 0;
	while(i < count && current)
	{
		new_token = create_token_copy(current);
		if(!new_token)
			return (NULL);
		if(!new_list)
			new_list = new_token;
		else
			last->next =new_token;
		last = new_token;
		current = current->next;
		i++;
	}
	return (new_list);
}

segment_t	*create_single_segment(token_t *start_token, int count)
{
	segment_t *segment;

	segment = create_segment();
	if(!segment)
		return(NULL);
	segment->token_count = count;
	segment->tokens = copy_tokens_until_pipe(start_token, count);
	if(!segment->tokens && count > 0)
	{
		free(segment);
		return (NULL);
	}
	return (segment);
}

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
		return (-1);
	*cmd_ptr = create_command();
	if (!*cmd_ptr)
		return (-1);
	current = segment->tokens;
	while(current)
	{
		if (is_redirect_token(*current))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
				return (parse_error(cmd_ptr));
			if (handle_redirect_pair(current, current->next, *cmd_ptr) != 0)
				return (parse_error(cmd_ptr));
		}
		else if (current->type == TOKEN_WORD)
		{
			if (handle_command_pair(current, *cmd_ptr) != 0)
				return (parse_error(cmd_ptr));
		}
		advance_to_next_token(&current);
	}
	return (0);
}

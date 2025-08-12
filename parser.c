/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:52:51 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 16:50:18 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_command_to_pipeline(command_t **pipeline_head, command_t *new_cmd)
{
	command_t	*current;

	if (!*pipeline_head)
	{
		*pipeline_head = new_cmd;
		return ;
	}
	current = *pipeline_head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

static command_t	*create_pipeline_from_segments(segment_t *segments)
{
	command_t	*pipeline_head;
	command_t	*new_command;
	segment_t	*current_segment;

	pipeline_head = NULL;
	current_segment = segments;
	while (current_segment)
	{
		new_command = NULL;
		if (parse_command_or_redirect(current_segment, &new_command) == -1)
		{
			free_pipeline(pipeline_head);
			return (NULL);
		}
		add_command_to_pipeline(&pipeline_head, new_command);
		current_segment = current_segment->next;
	}
	return (pipeline_head);
}

static token_t	*lex_and_validate(char *input)
{
	token_t	*tokens;

	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	if (!check_all_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

command_t	*parse_input(char *input)
{
	token_t		*tokens;
	segment_t	*segments;
	command_t	*pipeline_head;

	tokens = lex_and_validate(input);
	if (!tokens)
		return (NULL);
	segments = split_tokens_by_pipe(tokens);
	free_tokens(tokens);
	if (!segments)
		return (NULL);
	pipeline_head = create_pipeline_from_segments(segments);
	free_segments(segments); 
	return (pipeline_head);
}

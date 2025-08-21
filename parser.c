/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:52:51 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:41:55 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_command_to_pipeline(t_command **pipeline_head,
	t_command *new_cmd)
{
	t_command	*current;

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

static t_command	*create_pipeline_from_segments(t_segment *segments)
{
	t_command	*pipeline_head;
	t_command	*new_command;
	t_segment	*current_segment;

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

static t_token	*lex_and_validate(char *input)
{
	t_token	*tokens;

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

t_command	*parse_input(char *input)
{
	t_token		*tokens;
	t_segment	*segments;
	t_command	*pipeline_head;

	tokens = lex_and_validate(input);
	if (!tokens)
	{
		set_exit_code(2);
		return (NULL);
	}
	segments = split_tokens_by_pipe(tokens);
	free_tokens(tokens);
	if (!segments)
	{
		set_exit_code(2);
		return (NULL);
	}
	pipeline_head = create_pipeline_from_segments(segments);
	free_segments(segments);
	if (!pipeline_head)
	{
		set_exit_code(2);
		return (NULL);
	}
	return (pipeline_head);
}

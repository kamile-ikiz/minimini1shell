/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:53:17 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 15:30:28 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_tokens_until_pipe(t_token *token_list)
{
	int count;
	t_token *current;

	count = 0;
	current = token_list;
	while(current && current->type != TOKEN_PIPE)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static  t_segment *create_segment(void)
{
	t_segment	*segment;

	segment = malloc(sizeof(t_segment));
	if (!segment)
	{
		set_exit_code(1);
		return (NULL);
	}
	segment->tokens = NULL;
	segment->token_count = 0;
	segment->next = NULL;
	return (segment);
}

static  t_token	*create_token_copy(t_token *src)
{
	t_token	*new;
	new = malloc(sizeof(t_token));
	if(!new)
	{
		set_exit_code(1);
		return (NULL);
	}
	new->type = src->type;
	new->value = ft_strdup(src->value);
	if (!new->value && src->value)
	{
		free(new);
		set_exit_code(1);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

static  t_token *copy_tokens_until_pipe(t_token *start_token, int count)
{
	t_token *new_list;
	t_token *new_token;
	t_token *current;
	t_token *last;
	int i;

	new_list = NULL;
	last = NULL;
	current = start_token;
	i = 0;
	while(i < count && current)
	{
		new_token = create_token_copy(current);
		if(!new_token)
		{
			set_exit_code(1);
			return (NULL);
		}
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

t_segment	*create_single_segment(t_token *start_token, int count)
{
	t_segment *segment;

	segment = create_segment();
	if(!segment)
		return(NULL);
	segment->token_count = count;
	segment->tokens = copy_tokens_until_pipe(start_token, count);
	if(!segment->tokens && count > 0)
	{
		free(segment);
		set_exit_code(1);
		return (NULL);
	}
	return (segment);
}

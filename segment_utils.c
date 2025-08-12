/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:53:17 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 16:56:32 by kikiz            ###   ########.fr       */
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

static  segment_t *create_segment(void)
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

static  token_t	*create_token_copy(token_t *src)
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

static  token_t *copy_tokens_until_pipe(token_t *start_token, int count)
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

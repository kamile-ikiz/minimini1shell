/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:53:17 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 03:11:31 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_segment	*create_segment(void)
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

static	t_token	*create_token_copy(t_token *src)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		set_exit_code(1);
		return (NULL);
	}
	new->type = src->type;
	new->was_quoted = src->was_quoted;
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

static int	copy_and_append_token(t_token **new_list_ptr, t_token **last_ptr,
	t_token *src_token)
{
	t_token	*new_token;

	new_token = create_token_copy(src_token);
	if (!new_token)
		return (-1);
	if (*new_list_ptr == NULL)
		*new_list_ptr = new_token;
	else
		(*last_ptr)->next = new_token;
	*last_ptr = new_token;
	return (0);
}

t_token	*copy_tokens_until_pipe(t_token *start_token, int count)
{
	t_token	*new_list;
	t_token	*last;
	t_token	*current;
	int		i;

	new_list = NULL;
	last = NULL;
	current = start_token;
	i = 0;
	while (i < count && current)
	{
		if (copy_and_append_token(&new_list, &last, current) == -1)
		{
			free_tokens(new_list);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	return (new_list);
}

t_segment	*create_single_segment(t_token *start_token, int count)
{
	t_segment	*segment;

	segment = create_segment();
	if (!segment)
		return (NULL);
	segment->token_count = count;
	segment->tokens = copy_tokens_until_pipe(start_token, count);
	if (!segment->tokens && count > 0)
	{
		free(segment);
		set_exit_code(1);
		return (NULL);
	}
	return (segment);
}

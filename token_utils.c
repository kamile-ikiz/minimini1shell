/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:21:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 18:04:15 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value != NULL)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	token->was_quoted = 0;
	if (type == TOKEN_HEREDOC)
		token->expand_mode = 0;
	else
		token->expand_mode = 1;
	return (token);
}

t_token	*token_get_last(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

t_token	*token_lst(t_token **head, t_token *token)
{
	t_token	*last_node;

	if (!head)
		return (NULL);
	if (!token)
		return (*head);
	if (*head == NULL)
	{
		*head = token;
	}
	else
	{
		last_node = token_get_last(*head);
		last_node->next = token;
	}
	return (*head);
}

void	init_parser(t_parser *parser, char *input)
{
	parser->token_list = NULL;
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
}

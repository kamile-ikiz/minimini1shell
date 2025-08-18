/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:21:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/18 19:45:58 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_t	*new_token(token_type_t type, char *value)
{
	token_t *token = malloc(sizeof(token_t));
	if(!token)
		return(NULL);
	token->type = type;
	if(value != NULL)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	if (type == TOKEN_HEREDOC)
		token->expand_mode = 0;
	else
		token->expand_mode = 1;
	return (token);
}

token_t	*token_get_last(token_t *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

token_t	*token_lst(token_t **head, token_t *token)
{
	token_t	*last_node;

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

void	init_parser(parser_t *parser, char *input)
{
	parser->token_list = NULL;
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
}

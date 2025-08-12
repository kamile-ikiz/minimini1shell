/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:21:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 16:34:44 by kikiz            ###   ########.fr       */
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
	return (token);
}

void	token_lst(token_t **head, token_t *token)
{
	if(!*head)
		*head = token;
	else
	{
		token_t *current = *head;
		while(current->next)
			current = current->next;
		current->next = token;
	}
}

void	init_parser(parser_t *parser, char *input)
{
	parser->tokens = NULL;
	parser->current = NULL;
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
}

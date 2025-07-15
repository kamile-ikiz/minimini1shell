/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/15 19:53:45 by kikiz            ###   ########.fr       */
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
char *parse_quotes(parser_t *parser, char quote)
{
	int	start = ++parser->pos;
	int	len = 0;
	while(parser->inp[parser->pos] && parser->inp[parser->pos] != quote)
	{
		if(parser->inp[parser->pos] == '\\' && parser->inp[parser->pos + 1])
			parser->pos++;
		parser->pos++;
		len ++;
	}
	if(!parser->inp[parser->pos])
	{
		parser->error = 1;
		parser->error_msg = "Unclosed quote";
		return NULL;
	}
	char *res = ft_substr(parser->inp, start, parser->pos - start);
	parser->pos++;
	return(res);
}
//tokenize input string
token_t	*tokenize(char *input)
{
	parser_t parser = {0};
	parser.inp = input;
	parser.pos = 0;
	token_t *tokens = NULL;

	while(parser.inp[parser.pos])
	{
		skip_whitespace(&parser);
		if(!parser.inp[parser.pos])
			break;
		char c = parser.inp[parser.pos];
		token_t *token = NULL;
		if(c == '|' && parser.inp[parser.pos] == '|')
		{
			
		}
	}
		

}

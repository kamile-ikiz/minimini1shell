/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/21 19:38:05 by kikiz            ###   ########.fr       */
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
//INPUT TOKENIZE

static void	init_parser(parser_t *parser, char *input)
{
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
}

static token_t	*handle_operator_tokens(parser_t *parser)
{
	char	c;
	c = parser->inp[parser->pos];

	if (c == '|' && parser->inp[parser->pos + 1] != '|')
		return (parser->pos++, new_token(TOKEN_PIPE, "|"));
	if (c == '<')
	{
		if (parser->inp[parser->pos + 1] == '<')
			return (parser->pos += 2, new_token(TOKEN_HEREDOC, "<<"));
		return (parser->pos++, new_token(TOKEN_REDIRECT_IN, "<"));
	}
	if (c == '>')
	{
		if (parser->inp[parser->pos + 1] == '>')
			return (parser->pos += 2, new_token(TOKEN_REDIRECT_APPEND, ">>"));
		return (parser->pos++, new_token(TOKEN_REDIRECT_OUT, ">"));
	}
	if (c == '(')
		return (parser->pos++, new_token(TOKEN_LPAREN, "("));
	if (c == ')')
		return (parser->pos++, new_token(TOKEN_RPAREN, ")"));
	return (NULL);
}

static token_t	*handle_quoted_or_word(parser_t *parser)
{
	char	*val;
	token_t	*token;

	if (parser->inp[parser->pos] == '\'')
	{
		val = parse_quoted_string(parser, '\'');
		if (!val)
			return (new_token(TOKEN_ERROR, "quote_error"));
		token = new_token(TOKEN_WORD, val);
		free(val);
		return (token);
	}
	val = parse_word(parser);
	if (!val)
		return (NULL);
	token = new_token(TOKEN_WORD, val);
	free(val);
	return (token);
}

static int	process_token(parser_t *parser, token_t **tokens)
{
	token_t *token;

	skip_whitespace(parser);
	if (!parser->inp[parser->pos])
		return (0);
	token = handle_operator_tokens(parser);
	if (!token)
		token = handle_quoted_or_word(parser);
	if (token)
		token_lst(tokens, token);
	if (parser->error)
	{
		free_tokens(*tokens);
		return (-1);
	}
	return (1);
}

token_t	*tokenize(char *input)
{
	parser_t	parser = {0};
	token_t		*tokens;
	tokens = NULL;
	
	int			status;

	init_parser(&parser, input);
	while (parser.inp[parser.pos])
	{
		status = process_token(&parser, &tokens);
		if (status == -1)
			return (NULL);
		else if (status == 0)
			break ;
	}
	token_lst(&tokens, new_token(TOKEN_EOF, NULL));
	return (tokens);
}

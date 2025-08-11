/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/11 21:11:07 by kikiz            ###   ########.fr       */
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
	parser->tokens = NULL;
	parser->current = NULL;
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
	parser->error_msg = NULL;
}

char	*process_segment(parser_t *parser, char *final_val)
{
	char	*segment;
	char	c;

	c = parser->inp[parser->pos];
	if (c == '\'' || c == '"')
	{
		segment = parse_quotes(parser, c);
		if (!segment)
		{
			free(final_val);
			return (NULL);
		}
	}
	else
		segment = parse_unquoted_segment(parser);
	return (ft_join_and_free(final_val, segment));
}

static token_t	*handle_operator_tokens(parser_t *parser)
{
	char	c;
	c = parser->inp[parser->pos];

	if (c == '|')
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
	return (NULL);
}

token_t	*handle_quoted_or_word(parser_t *parser)
{
	char	*final_val;
	token_t	*token;

	final_val = ft_strdup("");
	if (!final_val)
		return (set_parser_error(parser, "malloc error", NULL));
	while (!is_word_delimiter(parser->inp[parser->pos]))
	{
		final_val = process_segment(parser, final_val);
		if (!final_val)
			return (NULL);
	}
	if (final_val[0] == '\0')
	{
		free(final_val);
		return (NULL);
	}
	token = new_token(TOKEN_WORD, final_val);
	if (!token)
		return (set_parser_error(parser, "malloc error", final_val));
	free(final_val);
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
	if (parser->error || !token)
	{
		ft_putendl_fd(parser->error_msg, 2);
		free_tokens(*tokens);
		return (-1);
	}
	return (1);
}

token_t	*tokenize(char *input)
{
	parser_t	parser;
	token_t		*tokens;
	int			status;

	tokens = (NULL);

	init_parser(&parser, input);
	while (parser.inp[parser.pos])
	{
		status = process_token(&parser, &tokens);
		if (status == -1)
			return (NULL);
		else if (status == 0)
			break ;
	}
	return (tokens);
}

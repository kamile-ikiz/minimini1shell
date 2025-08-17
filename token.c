/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/17 18:01:38 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	parse_and_append_segment(parser_t *parser, char **final_word)
{
	char	*segment;
	char	*temp;
	char	quote_char;
	token_t	*last_token;

	quote_char = parser->inp[parser->pos];
	last_token = NULL;
	if (parser->token_list != NULL)
		last_token = token_get_last(parser->token_list);
	if (quote_char == '\'' || quote_char == '"')
	{
		segment = parse_quotes(parser, quote_char);
		if (!segment)
			return (0);
		if (quote_char == '"')
		{
			if (last_token && last_token->expand_mode == 1)
			{
				temp = segment;
				segment = expand_all_variables(temp, init_env(NULL));
				free(temp);
			}
		}
	}
	else
	{
		temp = parse_unquoted_segment(parser);
		if (last_token && last_token->expand_mode == 1)
		{
			segment = expand_all_variables(temp, init_env(NULL));
			free(temp);
		}
		else
			segment = temp;
	}
		
	return (append_segment(final_word, segment));
}

static	token_t	*handle_word(parser_t *parser)
{
	char	*final_val;
	token_t	*token;

	final_val = ft_strdup("");
	if (!final_val)
		return (set_parser_error(parser, "malloc error", NULL));
	while (!is_word_delimiter(parser->inp[parser->pos]))
	{
		if(!parse_and_append_segment(parser, &final_val))
		{
			free(final_val);
			return (set_parser_error(parser, parser->error_msg, NULL));
		}
	}
	token = new_token(TOKEN_WORD, final_val);
	if (!token)
		return (set_parser_error(parser, "malloc error", final_val));
	//free(final_val);
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
		token = handle_word(parser);
	if (token)
		parser->token_list = token_lst(tokens, token);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/24 21:35:48 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_operator_tokens(t_parser *parser)
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

int	handle_quoted_segment(t_parser *parser, char **segment,
	t_token *last_token)
{
	char	quote_char;
	char	*temp;

	quote_char = parser->inp[parser->pos];
	*segment = parse_quotes(parser, quote_char);
	if (!*segment)
		return (0);
	if (quote_char == '"' && last_token && last_token->expand_mode == 1)
	{
		temp = *segment;
		*segment = expand_all_variables(temp, init_env(NULL));
		free(temp);
	}
	return (1);
}

int	handle_unquoted_segment(t_parser *parser, char **segment,
	t_token *last_token)
{
	char	*temp;

	temp = parse_unquoted_segment(parser);
	if (last_token && last_token->expand_mode == 1)
	{
		*segment = expand_all_variables(temp, init_env(NULL));
		free(temp);
	}
	else
		*segment = temp;
	return (1);
}

static int	process_token(t_parser *parser, t_token **tokens)
{
	t_token	*token;

	skip_whitespace(parser);
	if (!parser->inp[parser->pos])
		return (0);
	token = handle_operator_tokens(parser);
	if (!token)
		token = handle_word(parser);
	if (token)
		parser->token_list = token_lst(tokens, token);
	if (parser->error || !token || !parser->token_list)
	{
		if (parser->error_msg)
		{
			ft_putendl_fd(parser->error_msg, 2);
			free(parser->error_msg);
		}
		if (!token)
			free_tokens(token);
		if (!parser->token_list)
			free_tokens(parser->token_list);
		return (-1);
	}
	return (1);
}

t_token	*tokenize(char *input)
{
	t_parser	parser;
	t_token		*tokens;
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

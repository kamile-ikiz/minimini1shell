/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 22:11:57 by kikiz            ###   ########.fr       */
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

static int	handle_quoted_segment(t_parser *parser, char **segment, t_token *last_token)
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

static int	handle_unquoted_segment(t_parser *parser, char **segment, t_token *last_token)
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

static int	parse_and_append_segment(t_parser *parser, char **final_word)
{
	char	*segment;
	t_token	*last_token;

	segment = NULL;
	last_token = NULL;
	if (parser->token_list != NULL)
		last_token = token_get_last(parser->token_list);
	if (parser->inp[parser->pos] == '\'' || parser->inp[parser->pos] == '"')
	{
		if (!handle_quoted_segment(parser, &segment, last_token))
			return (0);
	}
	else
	{
		if (!handle_unquoted_segment(parser, &segment, last_token))
			return (0);
	}
	return (append_segment(final_word, segment));
}

static	t_token	*handle_word(t_parser *parser)
{
	char	*final_val;
	t_token	*token;
	t_token	*first_token;

	first_token = new_token(TOKEN_WORD, "");
	parser->token_list = first_token;
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
	//free(final_val);
	if (!token)
		return (set_parser_error(parser, "malloc error", final_val));
	return (token);
}

static int	process_token(t_parser *parser, t_token **tokens)
{
	t_token *token;

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

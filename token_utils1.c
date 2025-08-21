/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:57:26 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 05:03:27 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	build_final_word(t_parser *parser, char **final_val_ptr,
	int *has_quotes)
{
	while (!is_word_delimiter(parser->inp[parser->pos]))
	{
		if (parser->inp[parser->pos] == '\"'
			|| parser->inp[parser->pos] == '\'')
			*has_quotes = 1;
		if (!parse_and_append_segment(parser, final_val_ptr))
			return (-1);
	}
	return (0);
}

t_token	*handle_word(t_parser *parser)
{
	char	*final_val;
	int		has_quotes;
	t_token	*token;

	has_quotes = 0;
	parser->token_list = new_token(TOKEN_WORD, "");
	final_val = ft_strdup("");
	if (!final_val || !parser->token_list)
		return (set_parser_error(parser, "malloc error", final_val));
	if (build_final_word(parser, &final_val, &has_quotes) == -1)
	{
		free(final_val);
		return (set_parser_error(parser, parser->error_msg, NULL));
	}
	token = new_token(TOKEN_WORD, final_val);
	if (!token)
		return (set_parser_error(parser, "malloc error", final_val));
	free(final_val);
	token->was_quoted = has_quotes;
	return (token);
}

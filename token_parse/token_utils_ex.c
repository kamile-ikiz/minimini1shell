/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_ex.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:57:26 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 13:19:25 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dispatch_segment_parser(t_parser *parser, char **segment,
	t_token *last_token)
{
	if (parser->inp[parser->pos] == '\'' || parser->inp[parser->pos] == '\"')
		return (handle_quoted_segment(parser, segment, last_token));
	else
		return (handle_unquoted_segment(parser, segment, last_token));
}

static void	free_temp_token(t_token *token)
{
	if (token)
	{
		free(token->value);
		free(token);
	}
}

static int	parse_and_append_segment(t_parser *parser, char **final_word)
{
	char	*segment;
	t_token	*last_token;
	bool	is_new_token;

	segment = NULL;
	last_token = get_context_token(parser, &is_new_token);
	if (!last_token)
		return (0);
	if (dispatch_segment_parser(parser, &segment, last_token) == 0)
	{
		if (is_new_token)
			free_temp_token(last_token);
		free_tokens(parser->token_list);
		return (0);
	}
	if (is_new_token)
		free_temp_token(last_token);
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
	final_val = ft_strdup("");
	if (!final_val)
		return (set_parser_error(parser, "malloc error", final_val));
	if (build_final_word(parser, &final_val, &has_quotes) == -1)
	{
		free(final_val);
		if (parser->error)
			return (NULL);
		return (set_parser_error(parser, parser->error_msg, NULL));
	}
	token = new_token(TOKEN_WORD, final_val);
	if (!token)
		return (set_parser_error(parser, "malloc error", final_val));
	free(final_val);
	token->was_quoted = has_quotes;
	return (token);
}

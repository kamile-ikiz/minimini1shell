/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:47:41 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 16:50:44 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_error(command_t **cmd_ptr)
{
	if (cmd_ptr && *cmd_ptr)
	{
		free_command(*cmd_ptr);
		*cmd_ptr = NULL;
	}
	return (-1);
}

void	*set_parser_error(parser_t *parser, char *msg, void *to_free)
{
	if (to_free)
		free(to_free);
	parser->error = 1;
	parser->error_msg = ft_strjoin("bash: parser error: ", msg);
	return (NULL);
}

char	*parse_unquoted_segment(parser_t *parser)
{
	int	start;

	start = parser->pos;
	while (parser->inp[parser->pos]
		&& !is_space(parser->inp[parser->pos])
		&& parser->inp[parser->pos] != '\''
		&& parser->inp[parser->pos] != '"'
		&& parser->inp[parser->pos] != '|'
		&& parser->inp[parser->pos] != '<'
		&& parser->inp[parser->pos] != '>')
	{
		parser->pos++;
	}
	if (parser->pos == start)
		return (NULL);
	return (ft_substr(parser->inp, start, parser->pos - start));
}

char *parse_quotes(parser_t *parser, char quote)
{
	int	start;
	char *res;

	start = ++parser->pos;
	while(parser->inp[parser->pos] && parser->inp[parser->pos] != quote)
		parser->pos++;
	if(!parser->inp[parser->pos])
	{
		set_parser_error(parser, "unclosed quote!", NULL);
		return (NULL);
	}
	res = (ft_substr(parser->inp, start, parser->pos - start));
	parser->pos++;
	return(res);
}

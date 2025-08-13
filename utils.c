/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:41:07 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/13 19:51:04 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

void	skip_whitespace(parser_t *parser)
{
	while (parser->inp[parser->pos] && is_space(parser->inp[parser->pos]))
	{
		parser->pos++;
	}
}

int	is_word_delimiter(char c)
{
	if (is_space(c) || c == '|' || c == '<' || c == '>')
		return (1);
	if (c == '\0')
		return (1);
	return (0);
}

int	append_segment(char **word_ptr, char *segment)
{
	char	*temp_word;

	if (!segment)
		return (1);
	temp_word = ft_strjoin(*word_ptr, segment);
	free(*word_ptr);
	free(segment);
	*word_ptr = temp_word;
	if (!*word_ptr)
		return (0);
	return (1);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax  near unexpected token ", 2);
	ft_putendl_fd(token, 2);
}

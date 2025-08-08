/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 19:41:07 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/08 21:15:51 by beysonme         ###   ########.fr       */
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

static int	get_clean_len(const char *s)
{
	int		len;
	int		i;
	int		sq;
	int		dq;

	i = 0;
	len = 0;
	sq = 0;
	dq = 0;
	while (s[i])
	{
		if (s[i] == '\'' && dq == 0)
			sq = !sq;
		else if (s[i] == '\"' && sq == 0)
			dq = !dq;
		else
			len++;
		i++;
	}
	return (len);
}

static void	copy_without_quotes(char *dst, const char *src)
{
	int		i;
	int		j;
	int		sq;
	int		dq;

	i = 0;
	j = 0;
	sq = 0;
	dq = 0;
	while (src[i])
	{
		if (src[i] == '\'' && dq == 0)
			sq = !sq;
		else if (src[i] == '\"' && sq == 0)
			dq = !dq;
		else
			dst[j++] = src[i];
		i++;
	}
	dst[j] = '\0';
}

char	*remove_quotes(const char *input)
{
	int		clean_len;
	char	*result;

	if (!input)
		return (NULL);
	clean_len = get_clean_len(input);
	result = malloc(clean_len + 1);
	if (!result)
		return (NULL);
	copy_without_quotes(result, input);
	return (result);
}
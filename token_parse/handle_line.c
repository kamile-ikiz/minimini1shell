/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:52:02 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 01:52:41 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static int	has_unclosed_quotes(const char *line)
{
	int		i;
	char	quote_char;

	i = 0;
	quote_char = 0;
	while (line[i])
	{
		if (quote_char == 0 && is_quote(line[i]))
		{
			quote_char = line[i];
		}
		else if (quote_char != 0 && line[i] == quote_char)
		{
			quote_char = 0;
		}
		i++;
	}
	if (quote_char != 0)
		return (1);
	return (0);
}

int	handle_line(char *line)
{
	if (has_unclosed_quotes(line))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
		set_exit_code(2);
		return (1);
	}
	return (process_input_line(line));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:29:25 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 05:27:41 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_char(char *line, int *line_len, char c)
{
	if (*line_len >= 1023)
		return (0);
	line[(*line_len)++] = c;
	return (1);
}

static int	read_to_buffer(int fd, t_gnl_state *state)
{
	state->size = read(fd, state->buffer, BUFFER_SIZE);
	state->pos = 0;
	return (state->size);
}

static int	fill_line(int fd, char *line, int *line_len, t_gnl_state *state)
{
	while (1)
	{
		if (state->pos >= state->size)
		{
			if (read_to_buffer(fd, state) <= 0)
				break ;
		}
		if (!append_char(line, line_len, state->buffer[state->pos]))
			break ;
		state->pos++;
		if (line[*line_len - 1] == '\n')
			break ;
	}
	return (*line_len);
}

char	*get_next_line(int fd)
{
	static t_gnl_state	state = {.pos = 0, .size = 0};
	char				*line;
	int					line_len;

	line = malloc(1024);
	if (!line)
		return (NULL);
	line_len = 0;
	if (fill_line(fd, line, &line_len, &state) == 0)
	{
		free(line);
		return (NULL);
	}
	line[line_len] = '\0';
	return (line);
}

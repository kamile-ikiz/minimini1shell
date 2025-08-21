/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:29:25 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:37:08 by kikiz            ###   ########.fr       */
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

static int	read_to_buffer(int fd, char *buffer, int *buffer_pos)
{
	int	read_size;

	read_size = read(fd, buffer, BUFFER_SIZE);
	*buffer_pos = 0;
	return (read_size);
}

static int	fill_line(int fd, char *line, char *buffer, int *pos_size[2])
{
	int	*pos;
	int	*size;

	pos = pos_size[0];
	size = pos_size[1];
	while (1)
	{
		if (*pos >= *size)
		{
			*size = read_to_buffer(fd, buffer, pos);
			if (*size <= 0)
				break ;
		}
		if (!append_char(line, &pos_size[2][0], buffer[(*pos)++]))
			break ;
		if (line[pos_size[2][0] - 1] == '\n')
			break ;
	}
	return (pos_size[2][0]);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;
	int			*pos_size[3];

	line = malloc(1024);
	if (!line)
		return (NULL);
	line_len = 0;
	pos_size[0] = &buffer_pos;
	pos_size[1] = &buffer_size;
	pos_size[2] = &line_len;
	if (fill_line(fd, line, buffer, pos_size) == 0)
	{
		free(line);
		return (NULL);
	}
	line[line_len] = '\0';
	return (line);
}

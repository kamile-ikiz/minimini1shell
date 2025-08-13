/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:20:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/13 13:56:46 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_heredoc_line(char *line, int fd)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(fd, line, len) == -1 || write(fd, "\n", 1) == -1)
	{
		perror("write");
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

static int	process_line(char **line, int expand_vars, int fd)
{
	char	*expanded;

	if (expand_vars)
	{
		expanded = expand(*line);
		free(*line);
		*line = expanded;
	}
	return (write_heredoc_line(*line, fd));
}

static int	read_loop(const char *delimiter, int fd, int expand_vars)
{
	char	*line;
	size_t	len;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (-1);
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (process_line(&line, expand_vars, fd) == -1)
			return (-1);
	}
	return (0);
}

int	read_heredoc_until_delimiter(const char *delimiter,
		int write_fd, int expand_vars)
{
	return (read_loop(delimiter, write_fd, expand_vars));
}
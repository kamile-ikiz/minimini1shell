/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:56:20 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/20 18:02:34 by kikiz            ###   ########.fr       */
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

static void	handle_heredoc_child(const char *delimiter, int write_fd,
		int expand_vars)
{
	char	*line;
	size_t	len;

	configure_heredoc_signals();
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			write(1, "\n", 1);
			exit(0);
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			exit(0);
		}
		if (process_line(&line, expand_vars, write_fd) == -1)
			exit(1);
	}
}

int	read_heredoc_until_delimiter(const char *delimiter,
		int write_fd, int expand_vars)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
    {
        status = 0;
		handle_heredoc_child(delimiter, write_fd, expand_vars);
    }
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal_flag = SIGINT;
	}
	return (status);
}

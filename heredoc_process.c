/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:56:20 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/24 19:19:48 by beysonme         ###   ########.fr       */
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
		line = readline("> ");
		if (!line)
		{
			free_heredoc(NULL);
			write(1, "\n", 1);
			exit(0);
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free_heredoc(NULL);
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
		configure_prompt_signals();
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
			g_signal_flag = 2;
	}
	return (status);
}

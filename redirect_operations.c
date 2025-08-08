/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:00:57 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/07 17:09:37 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_child_process(int pipe_fd[2], char *delimiter)
{
	char	*line;

	close(pipe_fd[0]);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

int	heredoc_parent_process(int pipe_fd[2], pid_t pid)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		perror("dup2");
		return (-1);
	}
	close(pipe_fd[0]);
	return (0);
}

int	handle_heredoc_redirect(char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!delimiter)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		return (heredoc_child_process(pipe_fd, delimiter));
	return (heredoc_parent_process(pipe_fd, pid));
}

int	handle_input_redirect(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_append_redirect(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("dup2");
		return (-1);
	}
	close(fd);
	return (0);
}


int	handle_single_redirect(redirect_t *redirect)
{
	if (!redirect)
		return (-1);
	if (redirect->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirect(redirect->filename));
	else if (redirect->type == TOKEN_REDIRECT_IN)
		return (handle_input_redirect(redirect->filename));
	else if (redirect->type == TOKEN_REDIRECT_OUT)
		return (handle_output_redirect(redirect->filename));
	else if (redirect->type == TOKEN_REDIRECT_APPEND)
		return (handle_append_redirect(redirect->filename));
	return (-1);
}

int	execute_redirects(command_t *cmd)
{
	redirect_t	*redirects;
	redirect_t	*current;

	redirects = cmd->redirects;
	if (!redirects)
		return (0);
	current = redirects;
	while (current)
	{
		if (handle_single_redirect(current) != 0)
			return (-1);
		current = current->next;
	}
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:00:57 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/16 20:23:25 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
int execute_redirects(command_t *cmd)
{
    redirect_t *redir = cmd->redirects;

    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
        {
            if (dup2(redir->heredoc_pipe_fd, STDIN_FILENO) == -1)
            {
                perror("dup2 heredoc");
                close(redir->heredoc_pipe_fd);
                return -1;
            }
            close(redir->heredoc_pipe_fd); // dup2’den sonra orijinal fd kapatılır
            redir->heredoc_pipe_fd = -1;   // kapandığını belirt
        }
        else if (redir->type == TOKEN_REDIRECT_IN)
        {
            if (handle_input_redirect(redir->filename) == -1)
                return -1;
        }
        else if (redir->type == TOKEN_REDIRECT_OUT)
        {
            if (handle_output_redirect(redir->filename) == -1)
                return -1;
        }
        else if (redir->type == TOKEN_REDIRECT_APPEND)
        {
            if (handle_append_redirect(redir->filename) == -1)
                return -1;
        }
        redir = redir->next;
    }
    return 0;
}


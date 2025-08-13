/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:20:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/13 19:22:16 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_redirect(redirect_t *redir)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	if (read_heredoc_until_delimiter(redir->filename,
			pipe_fd[1], 1) == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	redir->heredoc_pipe_fd = pipe_fd[0];
	return (0);
}

static int	process_cmd_heredocs(command_t *cmd)
{
	redirect_t	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (process_heredoc_redirect(redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(command_t *cmd_list)
{
	command_t	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (process_cmd_heredocs(cmd) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_heredoc_pipes(command_t *cmd_list)
{
	command_t	*cmd;
	redirect_t	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC
				&& redir->heredoc_pipe_fd != -1)
			{
				close(redir->heredoc_pipe_fd);
				redir->heredoc_pipe_fd = -1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

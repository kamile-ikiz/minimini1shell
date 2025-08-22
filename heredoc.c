/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:54:08 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/22 22:11:05 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_heredoc(t_command *cmd)
{
	static t_command *tmp;

	if (cmd)
		tmp = cmd;
	if (!cmd && tmp)
	{
		free_environment(init_env(NULL));
		free_commands(tmp);
	}
}

static int	process_heredoc_redirect(t_redirect *redir)
{
	int	pipe_fd[2];
	int	status;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	status = read_heredoc_until_delimiter(redir->filename, pipe_fd[1], 1);
	close(pipe_fd[1]);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipe_fd[0]);
		redir->heredoc_pipe_fd = -1;
		return (status);
	}
	redir->heredoc_pipe_fd = pipe_fd[0];
	return (0);
}

static int	process_cmd_heredocs(t_command *cmd)
{
	t_redirect	*redir;
	int			status;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			status = process_heredoc_redirect(redir);
			if (status == -1)
				return (-1);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
				return (status);
		}
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(t_command *cmd_list)
{
	t_command	*cmd;
	int			status;

	cmd = cmd_list;
	free_heredoc(cmd_list);
	while (cmd)
	{
		status = process_cmd_heredocs(cmd);
		if (status == -1 || (WIFEXITED(status) && WEXITSTATUS(status) == 130))
		{
			return (-1);
		}
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_heredoc_pipes(t_command *cmd_list)
{
	t_command	*cmd;
	t_redirect	*redir;


	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->heredoc_pipe_fd != -1)
			{
				close(redir->heredoc_pipe_fd);
				redir->heredoc_pipe_fd = -1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

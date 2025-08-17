/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:20:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/16 22:33:39 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int process_heredoc_redirect(redirect_t *redir) 
{
    int pipe_fd[2];
    int status;

    if (pipe(pipe_fd) == -1)
        return (perror("pipe"), -1);

    status = read_heredoc_until_delimiter(redir->filename, pipe_fd[1], 1);
    close(pipe_fd[1]); // Yazma ucunu kapat

if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) ||
    (WIFEXITED(status) && WEXITSTATUS(status) == 130))
{
    close(pipe_fd[0]);
    redir->heredoc_pipe_fd = -1;
    return status; // Ctrl+C veya signal durumu
}

// Normal bitiş
redir->heredoc_pipe_fd = pipe_fd[0];
return 0;
}


static int	process_cmd_heredocs(command_t *cmd)
{
	redirect_t	*redir;
	int status;

	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			status = process_heredoc_redirect(redir);
			if (status == -1)
				return (-1);
			if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) ||
    			(WIFEXITED(status) && WEXITSTATUS(status) == 130))
    			return status; // heredoc iptal → pipeline iptal et
		}
		redir = redir->next;
	}
	return (0);
}

int	prepare_heredocs(command_t *cmd_list)
{
	command_t	*cmd;
	int			status;

	cmd = cmd_list;
	while (cmd)
	{
		status = process_cmd_heredocs(cmd);
		if (status == -1)
		{
			return (-1);
		}
		if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) ||
    		(WIFEXITED(status) && WEXITSTATUS(status) == 130))
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

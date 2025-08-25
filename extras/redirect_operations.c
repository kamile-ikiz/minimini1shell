/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_operations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:00:57 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 07:32:31 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	apply_heredoc_redirect(t_redirect *redir)
{
	if (dup2(redir->heredoc_pipe_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(redir->heredoc_pipe_fd);
		set_exit_code(1);
		return (-1);
	}
	close(redir->heredoc_pipe_fd);
	redir->heredoc_pipe_fd = -1;
	return (0);
}

static int	apply_file_redirect(t_redirect *redir)
{
	if (redir->type == TOKEN_REDIRECT_IN)
	{
		if (handle_input_redirect(redir->filename) == -1)
			return (-1);
	}
	else if (redir->type == TOKEN_REDIRECT_OUT)
	{
		if (handle_output_redirect(redir->filename) == -1)
			return (-1);
	}
	else if (redir->type == TOKEN_REDIRECT_APPEND)
	{
		if (handle_append_redirect(redir->filename) == -1)
			return (-1);
	}
	return (0);
}

static int	process_single_redirect(t_redirect *redir)
{
	if (redir->type == TOKEN_HEREDOC)
		return (apply_heredoc_redirect(redir));
	else
		return (apply_file_redirect(redir));
}

int	execute_redirects(t_command *cmd)
{
	t_redirect	*redir;

	redir = cmd->redirects;
	while (redir)
	{
		if (process_single_redirect(redir) == -1)
			return (-1);
		redir = redir->next;
	}
	return (1);
}

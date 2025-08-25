/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:50:09 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 08:44:40 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count)
{
	if (cmd_index > 0)
		dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < cmd_count - 1)
		dup2(pipes[cmd_index][1], STDOUT_FILENO);
}

void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	status;
	int	exit_status;
	int	i;

	exit_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFSIGNALED(status))
				exit_status = 130;
			else
				exit_status = WEXITSTATUS(status);
		}
		i++;
	}
	return (exit_status);
}

void	cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count)
{
	int	i;

	if (pipes)
	{
		i = 0;
		while (i < cmd_count - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
			i++;
		}
		free(pipes);
	}
	free(pids);
}

int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

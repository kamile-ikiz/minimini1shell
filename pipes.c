/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:40:57 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:21:40 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_failed_pipes(int **pipes, int failed_index)
{
	while (--failed_index >= 0)
	{
		close(pipes[failed_index][0]);
		close(pipes[failed_index][1]);
		free(pipes[failed_index]);
	}
	free(pipes);
}

int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	if (pipe_count <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free_failed_pipes(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

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
			exit_status = WEXITSTATUS(status);
		i++;
	}
	return (exit_status);
}

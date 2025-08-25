/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:51:38 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 08:41:17 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	**cleanup_pipes_on_error(int **pipes, int count)
{
	while (--count >= 0)
	{
		close(pipes[count][0]);
		close(pipes[count][1]);
		free(pipes[count]);
	}
	free(pipes);
	return (NULL);
}

static int	**create_pipes(int pipe_count)
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
			return (cleanup_pipes_on_error(pipes, i));
		}
		i++;
	}
	return (pipes);
}

static int	init_pipeline_data(t_command *cmd, t_pipeline_data *data)
{
	data->head = cmd;
	data->cmd_count = count_commands(cmd);
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
		return (-1);
	if (data->cmd_count > 1)
	{
		data->pipes = create_pipes(data->cmd_count - 1);
		if (!data->pipes)
		{
			free(data->pids);
			return (-1);
		}
	}
	else
		data->pipes = NULL;
	return (0);
}

static int	launch_processes(t_command *cmd, t_pipeline_data *data)
{
	int	i;

	i = 0;
	while (cmd && i < data->cmd_count)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
		{
			perror("fork");
			return (-1);
		}
		if (data->pids[i] == 0)
			pipeline_child_routine(cmd, data, i);
		cmd = cmd->next;
		i++;
	}
	return (0);
}

int	execute_pipeline(t_command *cmd)
{
	t_pipeline_data	data;
	int				exit_status;

	if (init_pipeline_data(cmd, &data) == -1)
		return (1);
	if (launch_processes(cmd, &data) == -1)
	{
		cleanup_pipeline(data.pids, data.pipes, data.cmd_count);
		return (1);
	}
	close_all_pipes(data.pipes, data.cmd_count - 1);
	configure_execution_signals();
	exit_status = wait_for_children(data.pids, data.cmd_count);
	configure_prompt_signals();
	cleanup_pipeline(data.pids, data.pipes, data.cmd_count);
	set_exit_code(exit_status);
	return (exit_status);
}

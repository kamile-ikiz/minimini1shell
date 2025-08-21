/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:20:23 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:20:45 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count)
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

static void	run_child_process(t_command *cmd, int **pipes, int i, int cmd_count)
{
	restore_default_signals();
	setup_pipe_redirections(pipes, i, cmd_count);
	close_all_pipes(pipes, cmd_count - 1);
	if (execute_redirects(cmd) == -1)
		exit(1);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd));
	if (cmd->args[0] != NULL)
		execve_command(cmd->args);
	exit(0);
}

static int	fork_and_exec_commands(t_command *cmd, int cmd_count,
				pid_t *pids, int **pipes)
{
	int	i;

	i = 0;
	while (cmd && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			cleanup_pipeline(pids, pipes, cmd_count);
			return (-1);
		}
		if (pids[i] == 0)
			run_child_process(cmd, pipes, i, cmd_count);
		cmd = cmd->next;
		i++;
	}
	return (0);
}

static int	count_commands(t_command *cmd)
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

int	execute_pipeline(t_command *cmd)
{
	int		cmd_count;
	pid_t	*pids;
	int		**pipes;
	int		exit_status;

	cmd_count = count_commands(cmd);
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		return (1);
	pipes = create_pipes(cmd_count - 1);
	if (!pipes && cmd_count > 1)
	{
		free(pids);
		return (1);
	}
	if (fork_and_exec_commands(cmd, cmd_count, pids, pipes) == -1)
		return (1);
	close_all_pipes(pipes, cmd_count - 1);
	configure_execution_signals();
	exit_status = wait_for_children(pids, cmd_count);
	cleanup_pipeline(pids, pipes, cmd_count);
	set_exit_code(exit_status);
	return (exit_status);
}

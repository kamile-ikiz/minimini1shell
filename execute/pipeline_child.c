/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:03:56 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 08:40:31 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_and_exit(t_pipeline_data *data, int exit_status)
{
	free_commands(data->head);
	cleanup_pipeline(data->pids, data->pipes, data->cmd_count);
	free_environment(init_env(NULL));
	exit(exit_status);
}

static void	child_run_external(t_command *cmd, t_pipeline_data *data)
{
	int	exit_status;

	exit_status = execve_command(cmd->args);
	cleanup_and_exit(data, exit_status);
}

static void	child_run_builtin(t_command *cmd, t_pipeline_data *data)
{
	int	exit_status;

	exit_status = execute_builtin(cmd);
	cleanup_and_exit(data, exit_status);
}

void	pipeline_child_routine(t_command *cmd, t_pipeline_data *data, int i)
{
	restore_default_signals();
	setup_pipe_redirections(data->pipes, i, data->cmd_count);
	close_all_pipes(data->pipes, data->cmd_count - 1);
	if (execute_redirects(cmd) == -1)
		cleanup_and_exit(data, 1);
	if (cmd->args && cmd->args[0])
	{
		if (is_builtin(cmd->args[0]))
			child_run_builtin(cmd, data);
		else
			child_run_external(cmd, data);
	}
	cleanup_and_exit(data, 0);
}

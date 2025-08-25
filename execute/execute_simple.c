/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:10:44 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 07:11:22 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_process_routine(t_command *cmd)
{
	int	exec_status;

	restore_default_signals();
	if (cmd->redirects)
	{
		if (execute_redirects(cmd) == -1)
		{
			free_commands(cmd);
			free_environment(init_env(NULL));
			exit(1);
		}
	}
	if (cmd->args && cmd->args[0])
	{
		exec_status = execve_command(cmd->args);
		free_commands(cmd);
		free_environment(init_env(NULL));
		exit(exec_status);
	}
	free_commands(cmd);
	free_environment(init_env(NULL));
	exit(0);
}

static int	parent_process_routine(pid_t child_pid)
{
	int	status;

	configure_execution_signals();
	waitpid(child_pid, &status, 0);
	configure_prompt_signals();
	if (WIFSIGNALED(status))
	{
		status = 130;
		return (status);
	}
	else
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_simple_command(t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		child_process_routine(cmd);
	}
	return (parent_process_routine(pid));
}

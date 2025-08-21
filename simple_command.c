/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:34:14 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:24:39 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_child_simple(t_command *cmd)
{
	restore_default_signals();
	if (execute_redirects(cmd) == -1)
		exit(1);
	if (cmd->args[0] != NULL)
		execve_command(cmd->args);
	exit(0);
}

int	execute_simple_command(t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		run_child_simple(cmd);
	configure_execution_signals();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (status);
	return (1);
}

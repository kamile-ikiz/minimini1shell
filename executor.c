/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:44:55 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_builtin_with_redirects(t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	exit_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->redirects && execute_redirects(cmd) == -1)
		return (1);
	exit_status = execute_builtin(cmd);
	if (cmd->redirects)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	return (exit_status);
}

static int	execute_nonbuiltin(t_command *cmd)
{
	if (cmd->next)
		return (execute_pipeline(cmd));
	else
		return (execute_simple_command(cmd));
}

int	execute_command(t_command *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (cmd->args == NULL)
	{
		set_exit_code(0);
		return (0);
	}
	if (is_builtin(cmd->args[0]))
		exit_status = execute_builtin_with_redirects(cmd);
	else
		exit_status = execute_nonbuiltin(cmd);
	cleanup_heredoc_pipes(cmd);
	set_exit_code(exit_status);
	return (exit_status);
}

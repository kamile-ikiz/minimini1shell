/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:12:06 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 08:39:25 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	backup_and_apply_redirects(t_command *cmd, int *stdin_backup,
	int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		perror("dup");
		return (-1);
	}
	if (execute_redirects(cmd) == -1)
		return (-1);
	return (0);
}

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

int	execute_builtin_command(t_command *cmd)
{
	int	exit_status;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	if (cmd->redirects)
	{
		if (backup_and_apply_redirects(cmd, &saved_stdin, &saved_stdout) == -1)
			return (1);
	}
	exit_status = execute_builtin(cmd);
	if (saved_stdin != -1)
		restore_fds(saved_stdin, saved_stdout);
	if ((ft_strcmp(cmd->args[0], "exit") == 0)
		&& !(is_numeric(cmd->args[1]) && cmd->args[2]))
	{
		free_commands(cmd);
		free_environment(init_env(NULL));
		exit(exit_status);
	}
	return (exit_status);
}

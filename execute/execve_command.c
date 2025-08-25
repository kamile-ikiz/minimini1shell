/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 07:08:53 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 11:50:53 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_a_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0)
		return (S_ISDIR(statbuf.st_mode));
	return (0);
}

static int	handle_exec_error(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	if (is_a_directory(command))
	{
		ft_putendl_fd(": Is a directory", 2);
		return (126);
	}
	if (ft_strchr(command, '/'))
	{
		ft_putendl_fd(": No such file or directory", 2);
		return (127);
	}
	ft_putendl_fd(": command not found", 2);
	return (127);
}

static int	execute_direct_path(char **args, char **envp)
{
	int	exit_code;

	execve(args[0], args, envp);
	exit_code = handle_exec_error(args[0]);
	free_args(envp);
	free_environment(init_env(NULL));
	return (exit_code);
}

static int	execute_path_search(char **args, char **envp)
{
	char	*cmd_path;
	int		exit_code;

	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		exit_code = handle_exec_error(args[0]);
		free_args(envp);
		free_environment(init_env(NULL));
		return (exit_code);
	}
	execve(cmd_path, args, envp);
	exit_code = handle_exec_error(args[0]);
	free(cmd_path);
	free_args(envp);
	free_environment(init_env(NULL));
	return (exit_code);
}

int	execve_command(char **args)
{
	char	**envp;

	if (!args || !args[0])
		return (127);
	envp = env_list_to_envp(init_env(NULL));
	if (!envp && *init_env(NULL) != NULL)
		return (1);
	if (ft_strchr(args[0], '/'))
		return (execute_direct_path(args, envp));
	else
		return (execute_path_search(args, envp));
}

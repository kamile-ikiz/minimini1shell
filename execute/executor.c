/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 12:59:31 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = get_env_value("PATH", init_env(NULL));
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_args(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_args(paths);
	return (NULL);
}

int	execute_command(t_command *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (cmd == NULL)
		return (set_exit_code(0), 0);
	if (cmd->next)
		exit_status = execute_pipeline(cmd);
	else if (cmd->args && is_builtin(cmd->args[0]))
		exit_status = execute_builtin_command(cmd);
	else
		exit_status = execute_simple_command(cmd);
	cleanup_heredoc_pipes(cmd);
	set_exit_code(exit_status);
	return (exit_status);
}

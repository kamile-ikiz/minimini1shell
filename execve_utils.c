/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:35:40 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:18:25 by beysonme         ###   ########.fr       */
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

static void	handle_direct_path(char *arg)
{
	if (is_a_directory(arg))
	{
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(" : Is a directory", 2);
		exit(126);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit(127);
}

static char	*find_command_path(char *cmd)
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
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	execve_command(char **args)
{
	char	*cmd_path;
	char	**envp;

	envp = env_list_to_envp(init_env(NULL));
	if (!args || !args[0])
		exit(127);
	if (ft_strchr(args[0], '/'))
		handle_direct_path(args[0]);
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (execve(cmd_path, args, envp) == -1)
	{
		free(cmd_path);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	return (0);
}

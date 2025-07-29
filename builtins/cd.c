/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beyzasonmez565@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:44:51 by beysonme          #+#    #+#             */
/*   Updated: 2025/07/26 20:06:45 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_error(char *msg, char *to_free)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	if (to_free)
		free(to_free);
	return (1);
}

static char	*get_path(t_command *cmd, char *cwd)
{
	char	*path;

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		path = getenv("HOME");
	else
		path = cmd->args[1];
	if (!path)
	{
		print_error("cd: HOME not set\n", cwd);
		return (NULL);
	}
	return (path);
}

int	builtin_cd(t_command *cmd)
{
	char	*path;
	char	*cwd;

	if (cmd->args[1] && cmd->args[2])
		return (print_error("cd: too many arguments\n", NULL));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (print_error("cd: error getting current directory\n", NULL));
	path = get_path(cmd, cwd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		free(cwd);
		perror("cd");
		return (1);
	}
	free(cwd);
	return (0);
}

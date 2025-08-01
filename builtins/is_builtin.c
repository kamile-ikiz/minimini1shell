/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:46:37 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/01 16:11:17 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	char	*builtins[8];
	int		i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	if (!cmd)
		return (0);
	i = 0;
	while (builtins[i])
	{
		if (!ft_strcmp(cmd, builtins[i]))
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(command_t *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd(cmd));
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(cmd));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd));
	return (0);
}

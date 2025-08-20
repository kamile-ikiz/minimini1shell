/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:46:37 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/20 17:59:12 by kikiz            ###   ########.fr       */
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

int	execute_builtin(t_command *cmd)
{
	int	ret;

	ret = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		ret = builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		ret = builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ret = builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ret = builtin_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ret = builtin_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ret = builtin_env(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		ret = builtin_exit(cmd);
	set_exit_code(ret);
	return (ret);
}

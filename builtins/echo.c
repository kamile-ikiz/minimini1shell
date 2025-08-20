/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:16:42 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/20 17:59:26 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_n_option(const char *arg)
{
	if (arg == NULL || *arg != '-')
		return (0);
	arg++;
	while (*arg)
	{
		if (*arg != 'n')
			return (0);
		arg++;
	}
	return (1);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (cmd->args[i] && is_n_option(cmd->args[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], 1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	set_exit_code(0);
	return (0);
}

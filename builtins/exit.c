/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:31:08 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/17 21:02:48 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	builtin_exit(command_t *cmd)
{
	ft_putstr_fd("exit\n", 1);
	if (!cmd->args[1])
	{
		set_exit_code(0); // Başarılı durumda exit code 0
		exit(0);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		set_exit_code(255); // Hatalı numeric argument
		exit(255);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		set_exit_code(1); // Hata durumunda exit code 1
		return (1);
	}
	set_exit_code(ft_atoi(cmd->args[1])); // Çıkış kodunu ayarlıyoruz
	exit(ft_atoi(cmd->args[1]));
}

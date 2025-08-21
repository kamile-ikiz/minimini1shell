/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:22:57 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrupt_callback_execution(int signal_num)
{
	(void)signal_num;
	write(STDOUT_FILENO, "\n", 1);
	set_exit_code(130);
}

static void	quit_callback_execution(int signal_num)
{
	(void)signal_num;
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	set_exit_code(131);
}

void	configure_execution_signals(void)
{
	assign_signal_handler(SIGINT, interrupt_callback_execution);
	assign_signal_handler(SIGQUIT, quit_callback_execution);
}

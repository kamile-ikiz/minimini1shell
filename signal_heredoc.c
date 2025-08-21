/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:23:42 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrupt_callback_heredoc(int signal_num)
{
	(void)signal_num;
	g_signal_flag = SIGINT;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	configure_heredoc_signals(void)
{
	assign_signal_handler(SIGINT, interrupt_callback_heredoc);
	assign_signal_handler(SIGQUIT, SIG_IGN);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

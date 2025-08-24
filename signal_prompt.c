/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/24 17:01:42 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_flag = 0;

void	assign_signal_handler(int signal_type, void (*callback)(int))
{
	signal(signal_type, callback);
}

static void	interrupt_callback_prompt(int signal_num)
{
	(void)signal_num;
	write(STDOUT_FILENO, "\n", 1);
	set_exit_code(130);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	configure_prompt_signals(void)
{
	assign_signal_handler(SIGQUIT, SIG_IGN);
	assign_signal_handler(SIGINT, interrupt_callback_prompt);
}
		

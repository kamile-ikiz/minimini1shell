/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_prompt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:24:17 by beysonme         ###   ########.fr       */
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

int	configure_prompt_signals(void)
{
	static int	ctl;

	ctl = 0;
	if (ctl == 0)
	{
		assign_signal_handler(SIGINT, interrupt_callback_prompt);
		assign_signal_handler(SIGQUIT, SIG_IGN);
		ctl = 1;
	}
	return (ctl);
}

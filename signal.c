/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 17:43:40 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/17 19:02:33 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_flag = 0; 

void    assign_signal_handler(int signal_type, void (*callback)(int))
{
    signal(signal_type, callback);
}

//PROMPT

void	interrupt_callback_prompt(int signal_num)
{
	(void)signal_num;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int    configure_prompt_signals(void)
{
	static int ctl;

	ctl = 0;
	if (ctl == 0)
	{
		assign_signal_handler(SIGINT, interrupt_callback_prompt);
    	assign_signal_handler(SIGQUIT, SIG_IGN);
		ctl = 1;
	}
	return (ctl);
}

//EXECUTE

void	interrupt_callback_execution(int signal_num)
{
	(void)signal_num;
	//g_signal_flag = SIGINT<<;
	write(STDOUT_FILENO, "\n", 1);
}

void	quit_callback_execution(int signal_num)
{
	(void)signal_num;
	g_signal_flag = SIGQUIT;
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

void	configure_execution_signals(void)
{
	assign_signal_handler(SIGINT, interrupt_callback_execution);
	assign_signal_handler(SIGQUIT, quit_callback_execution);
}
//HEREDOC
void	interrupt_callback_heredoc(int signal_num)
{
	(void)signal_num;
	g_signal_flag = SIGINT;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void 	configure_heredoc_signals(void)
{
		assign_signal_handler(SIGINT, interrupt_callback_heredoc);
		assign_signal_handler(SIGQUIT, SIG_IGN);
}

// EXTRA

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
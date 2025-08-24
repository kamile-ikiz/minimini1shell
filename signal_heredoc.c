/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:10:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/24 18:20:49 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	interrupt_callback_heredoc(int signal_num)
{
	(void)signal_num;
	free_heredoc(NULL);
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	configure_heredoc_signals(void)
{
	signal(SIGINT, interrupt_callback_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

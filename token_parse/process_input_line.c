/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 01:44:28 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 01:47:35 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredocs_and_signals(t_command *pipeline)
{
	prepare_heredocs(pipeline);
	if (g_signal_flag == 2)
	{
		set_exit_code(130);
		cleanup_heredoc_pipes(pipeline);
		free_commands(pipeline);
		g_signal_flag = 0;
		return (1);
	}
	return (0);
}

static void	execute_and_cleanup(t_command *pipeline)
{
	execute_command(pipeline);
	cleanup_heredoc_pipes(pipeline);
	free_commands(pipeline);
}

int	process_input_line(char *line)
{
	t_command	*pipeline;

	if (*line == '\0')
	{
		set_exit_code(0);
		return (1);
	}
	add_history(line);
	pipeline = parse_input(line);
	if (pipeline)
	{
		if (handle_heredocs_and_signals(pipeline) == 1)
		{
			return (1);
		}
		execute_and_cleanup(pipeline);
	}
	return (0);
}

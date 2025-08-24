/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:39 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/24 17:01:37 by beysonme         ###   ########.fr       */
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

static int	process_input_line(char *line)
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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_env	**env_list;
	int		std_in;
	int		std_out;

	(void)argc;
	(void)argv;
	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	env_list = init_env(envp);
	while (1)
	{
		dup2(std_in, STDIN_FILENO);
		dup2(std_out, STDOUT_FILENO);
		configure_prompt_signals();
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (process_input_line(line) == 1)
		{
			free(line);
			continue ;
		}
		free(line);
	}
	free_environment(env_list);
	return (get_exit_code());
}

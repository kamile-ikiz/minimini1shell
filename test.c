/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:39 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 04:08:05 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static int	handle_heredocs_and_signals(t_command *pipeline)
{
	prepare_heredocs(pipeline);
	if (g_signal_flag == SIGINT)
	{
		set_exit_code(130);
		cleanup_heredoc_pipes(pipeline);
		free_pipeline(pipeline);
		g_signal_flag = 0;
		return (1); // Sinyal alındı, döngüye devam et
	}
	return (0); // Sorun yok, yürütmeye geç
}

static void	execute_and_cleanup(t_command *pipeline)
{
	execute_command(pipeline);
	cleanup_heredoc_pipes(pipeline);
	free_pipeline(pipeline);
}

static int	process_input_line(char *line)
{
	t_command	*pipeline;

	if (*line == '\0')
	{
		set_exit_code(0);
		return (1); // Boş satır, döngüye devam et
	}
	add_history(line);
	pipeline = parse_input(line);
	if (pipeline)
	{
		if (handle_heredocs_and_signals(pipeline) == 1)
			return (1); // Heredoc'ta sinyal geldi, döngüye devam et
		execute_and_cleanup(pipeline);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	init_env(envp);
	while (1)
	{
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
	return (get_exit_code());
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char		*line;
// 	int			status;
// 	t_command	*pipeline;

// 	(void)argc;
// 	(void)argv;
// 	init_env(envp);
// 	while (1) 
// 	{
// 		configure_prompt_signals();
// 		line = readline(PROMPT);
// 		if (!line) 
// 		{
// 			printf("exit\n");
// 			break;
// 		}
// 		else if(*line == '\0')
// 		{
// 			free(line);
// 			set_exit_code(0);
// 			continue;
// 		}
// 		if (*line)
// 			add_history(line);
		
// 		pipeline = parse_input(line);
// 		if (pipeline) 
// 		{
// 			status = prepare_heredocs(pipeline);
// 			if (g_signal_flag == SIGINT)
// 			{	
// 					set_exit_code(130);
// 					cleanup_heredoc_pipes(pipeline);
// 					free_pipeline(pipeline);
// 					free(line);
// 					g_signal_flag = 0;
// 					continue; // SIGINT durumunda komut çalıştırma}
			
// 			}
// 			// Ctrl+D (EOF) durumunda status = 0 olur ve komut çalışır
// 			execute_command(pipeline);
// 			cleanup_heredoc_pipes(pipeline);
// 			free_pipeline(pipeline);
// 		}
// 		free(line);
// 	}
// 	return (get_exit_code());
// }


#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	int			status;
	command_t	*pipeline;

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
			break;
		}
		else if(*line == '\0')
		{
			free(line);
			set_exit_code(0);
			continue;
		}
		if (*line)
			add_history(line);
		
		pipeline = parse_input(line);
		if (pipeline) 
		{
			status = prepare_heredocs(pipeline);
			if (g_signal_flag == SIGINT)
			{	
					set_exit_code(130);
					cleanup_heredoc_pipes(pipeline);
					free_pipeline(pipeline);
					free(line);
					g_signal_flag = 0;
					continue; // SIGINT durumunda komut çalıştırma}
			
			}
			// Ctrl+D (EOF) durumunda status = 0 olur ve komut çalışır
			execute_command(pipeline);
			cleanup_heredoc_pipes(pipeline);
			free_pipeline(pipeline);
		}
		free(line);
	}
	return (get_exit_code());
}

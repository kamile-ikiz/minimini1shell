
#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	command_t	*pipeline;

	(void)argc;
	(void)argv;
	configure_prompt_signals();
	init_env(envp);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		
		pipeline = parse_input(line);
		if (pipeline)
		{
			if (prepare_heredocs(pipeline) == -1)  // heredocları burada hazırla
			{
  				perror("heredoc preparation failed");
        		cleanup_heredoc_pipes(pipeline);  // varsa açık fd'leri kapat
        		free_pipeline(pipeline);
        		free(line);
        		continue; 
    		}
			execute_command(pipeline);
			cleanup_heredoc_pipes(pipeline);
			free_pipeline(pipeline);
		}
		free(line);
	}
	return (0);
}

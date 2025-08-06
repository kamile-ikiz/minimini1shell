
#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	command_t	*pipeline;

	// argc, argv, envp şimdilik kullanılmıyor, ileride lazım olacak.
	(void)argc;
	(void)argv;
	(void)envp;
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
		
		// 3. SONUCU KONTROL ET
		if (pipeline)
		{
			print_pipeline(pipeline);
			
			// !!! İLERİDE BURAYA EXECUTE FONKSİYONU GELECEK !!!
			// execute_pipeline(pipeline, envp);

			free_pipeline(pipeline); //belleği temizle
		}
		free(line);
	}
	return (0);
}
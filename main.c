/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:39 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 01:52:21 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_shell_loop_iteration(int std_in, int std_out)
{
	char	*line;

	dup2(std_in, STDIN_FILENO);
	dup2(std_out, STDOUT_FILENO);
	configure_prompt_signals();
	line = readline(PROMPT);
	if (!line)
	{
		printf("exit\n");
		return (1);
	}
	if (handle_line(line) == 1)
	{
		add_history(line);
		free(line);
		return (0);
	}
	free(line);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
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
		if (process_shell_loop_iteration(std_in, std_out) == 1)
			break ;
	}
	free_environment(env_list);
	close(std_in);
	close(std_out);
	return (get_exit_code());
}

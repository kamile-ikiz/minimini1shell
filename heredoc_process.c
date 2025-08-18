/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:20:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/18 20:36:40 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_heredoc_line(char *line, int fd)
{
	size_t	len;

	len = ft_strlen(line);
	if (write(fd, line, len) == -1 || write(fd, "\n", 1) == -1)
	{
		perror("write");
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

static int	process_line(char **line, int expand_vars, int fd)
{
	char	*expanded;

	if (expand_vars)
	{
		expanded = expand(*line);
		free(*line);
		*line = expanded;
	}
	return (write_heredoc_line(*line, fd));
}

static int read_loop(const char *delimiter, int fd, int expand_vars)
{
    char *line;
    size_t len;
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) // child
    {
        configure_heredoc_signals(); // SIGINT default değil, kendi handler
        while (1)
        {
            write(STDOUT_FILENO, "> ", 2);
            line = get_next_line(STDIN_FILENO);
            if (!line)
            {
                write(1,"\n",1);
                exit(0); // Ctrl+D veya EOF
            }

            len = ft_strlen(line);
            if (len > 0 && line[len - 1] == '\n')
                line[len - 1] = '\0';

            if (ft_strcmp(line, delimiter) == 0)
            {
                free(line);
                exit(0);
            }
            if (process_line(&line, expand_vars, fd) == -1)
                exit(1);
        }
    }
    else // parent
    {
        signal(SIGINT, SIG_IGN);
        waitpid(pid, &status, 0);

        // >>> SIGINT tespitini parent’a taşı
        if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) ||
            (WIFEXITED(status) && WEXITSTATUS(status) == 130))
        {
            g_signal_flag = SIGINT;
        }

        configure_prompt_signals();
        return (status); // raw status dönmeye devam et
    }
}




int	read_heredoc_until_delimiter(const char *delimiter,
		int write_fd, int expand_vars)
{
	return (read_loop(delimiter, write_fd, expand_vars));
}
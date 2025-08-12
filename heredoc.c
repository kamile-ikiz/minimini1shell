/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:06:31 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/12 19:25:56 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int read_heredoc_until_delimiter(const char *delimiter, int write_fd)
{
    char *line;

    while (1)
    {
        write(STDOUT_FILENO, "> ", 2);
        
        line = get_next_line(STDIN_FILENO);
        if (!line) // EOF veya hata
            return -1;

        // Satır sonundaki '\n' karakteri get_next_line tarafından dahil edilir,
        // delimiter karşılaştırması için kaldırabiliriz:
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break; // Delimiter bulundu, heredoc sonu
        }

        // Delimiter değil, pipe'a yaz
        len = strlen(line);
        if (write(write_fd, line, len) == -1)
        {
            perror("write");
            free(line);
            return -1;
        }
        if (write(write_fd, "\n", 1) == -1)
        {
            perror("write");
            free(line);
            return -1;
        }

        free(line);
    }
    return 0;
}


int prepare_heredocs(command_t *cmd_list)
{
    command_t *cmd = cmd_list;

    while (cmd)
    {
        redirect_t *redir = cmd->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC)
            {
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1)
                {
                    perror("pipe");
                    return -1;
                }

                if (read_heredoc_until_delimiter(redir->filename, pipe_fd[1]) == -1)
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                    return -1;
                }

                close(pipe_fd[1]); // yazma ucu kapatıldı, sadece okuma fd açık
                redir->heredoc_pipe_fd = pipe_fd[0];
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return 0;
}

void cleanup_heredoc_pipes(command_t *cmd_list)
{
    command_t *cmd = cmd_list;

    while (cmd)
    {
        redirect_t *redir = cmd->redirects;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC && redir->heredoc_pipe_fd != -1)
            {
                close(redir->heredoc_pipe_fd);
                redir->heredoc_pipe_fd = -1;
            }
            redir = redir->next;
        }
        cmd = cmd->next;
    }
}


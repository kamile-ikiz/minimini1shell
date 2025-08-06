/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 14:59:30 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/04 18:42:32 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    token_t     *token;
    command_t   *command;
    segment_t   *segments;
    char        *line;

    (void)argc;
    (void)argv;
    init_env(env);
    configure_prompt_signals();
    while (true)
    {
        line = readline(PROMPT);
        if (!line)
            return 1;
        command = NULL;
        token = NULL;
        segments = NULL;
        add_history(line);
        token = tokenize(line);
        if (!token)
            free(line);
        else if (check_all_syntax(token))
        {
            free(line);
            segments = split_tokens_by_pipe(token);
            if (segments && parse_command_or_redirect(segments, command))
            {
                // TOKEN FREE YAP ARTIK İHTİYACIN YOK
                // EXECUTE
            }
        }
        else
            free(line);
    }    
    return (0);
}
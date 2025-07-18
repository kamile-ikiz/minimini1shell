/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:16:14 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/18 17:33:09 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//new command
command_t *new_command(void)
{
    command_t *cmd = malloc(sizeof(command_t));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_mode = 0;
    cmd->heredoc_delimiter = NULL;
    cmd->next = NULL;
    return cmd;
}
void add_argmnt(command_t *cmd, char *arg)
{
    if (!cmd || !arg)
        return;
    cmd->args = realloc(cmd->args, sizeof(char*) * (cmd->argc + 2));
    if (!cmd->args)
        return;
    cmd->args[cmd->argc] = ft_strdup(arg);
    cmd->args[cmd->argc + 1] = NULL;
    cmd->argc++;
}
command_t *parse_command(parser_t *parser)
{
    command_t *cmd = new_command();
    if (!cmd)
        return (NULL);

    while (parser->inp[parser->pos] && !is_space(parser->inp[parser->pos]) &&
           parser->inp[parser->pos] != '|' && parser->inp[parser->pos] != '<' &&
           parser->inp[parser->pos] != '>' && parser->inp[parser->pos] != '&' &&
           parser->inp[parser->pos] != ';' && parser->inp[parser->pos] != '(' &&
           parser->inp[parser->pos] != ')') {
        char *word = parse_word(parser);
        if (word) {
            add_argmnt(cmd, word);
            free(word);
        } else {
            break;
        }
    }

    return cmd;
}

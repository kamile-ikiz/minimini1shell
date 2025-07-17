/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/18 00:09:15 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tokens(token_t *tokens) {
    while (tokens) {
        token_t *next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}

void free_command(command_t *cmd) {
    if (!cmd) return;
    
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd->heredoc_delimiter);
    free(cmd);
}

void free_pipeline(pipeline_t *pipeline) {
    while (pipeline) {
        pipeline_t *next_pipeline = pipeline->next;
        
        command_t *cmd = pipeline->commands;
        while (cmd) {
            command_t *next_cmd = cmd->next;
            free_command(cmd);
            cmd = next_cmd;
        }
        
        free(pipeline);
        pipeline = next_pipeline;
    }
}
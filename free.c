/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/04 20:06:14 by kikiz            ###   ########.fr       */
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

static void	free_args(char **args, int argc)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < argc)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_redirects(redirect_t *redirects)
{
	redirect_t	*temp;

	while (redirects)
	{
		temp = redirects;
		redirects = redirects->next;
		free(temp->filename);
		free(temp);
	}
}

void	free_command(command_t *cmd)
{
	if (!cmd)
		return ;
	free_args(cmd->args, cmd->argc);
	free_redirects(cmd->redirects);
	free(cmd);
}

void free_segments(segment_t *segments)
{
	segment_t	*temp;
	token_t		*token_temp;
	token_t		*token_next;

	while(segments)
	{
		temp = segments;
		segments = segments->next;
		while(temp->tokens)
		{
			token_temp = temp->tokens;
			token_next = temp->tokens->next;
			free(token_temp->value);
			free(token_temp);
			temp->tokens = token_next;
		}
		free(temp);
	}
}

void free_pipeline(pipeline_t *pipeline)
{
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


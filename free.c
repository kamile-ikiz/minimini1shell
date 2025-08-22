/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/22 13:06:56 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
}

void free_args(char **args)
{
	int i;

	i = 0;
	if(ft_strcmp(args[0], "export") == 0)
		free(args[0]);
	else
	{
		while (args[i])
		{
			free(args[i]);
			i++;
		}
	}
	free(args);
}

// void	free_redirects(t_redirect *redirects)
// {
// 	t_redirect	*temp;

// 	while (redirects)
// 	{
// 		temp = redirects;
// 		redirects = redirects->next;
// 		free(temp->filename);
// 		free(temp);
// 	}
// }

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*tmp;

	while (redirects)
	{
		tmp = redirects->next;
		if (redirects->filename)
			free(redirects->filename);
		if (redirects->heredoc_pipe_fd > 0)
			close(redirects->heredoc_pipe_fd);
		free(redirects);
		redirects = tmp;
	}
}
void free_commands(t_command *cmd)
{
    t_command *tmp;
    
    while (cmd != NULL)
    {
        tmp = cmd;          // Mevcut node'u sakla
        cmd = cmd->next;    // Sonraki node'a geç
        
        // İçeriği temizle
		if(tmp->args)
        	free_args(tmp->args);
		if(tmp->redirects)
        	free_redirects(tmp->redirects);
        
        // Node'u serbest bırak
        free(tmp);
    }
}

// void	free_command(t_command *cmd)
// {
// 	// while (cmd)
// 	// {
// 	// 	free_args(cmd->args, cmd->argc);
// 	// 	free_redirects(cmd->redirects);
// 	// 	cmd = cmd->next;
// 	// }
// 	// // free(cmd);
// 	// int i = 0;
// 	// while(cmd->args)
// 	// {
// 	// 	printf("burada : %s\n", cmd->args[i]);
// 	// 	i++;
// 	// }
// 	if (!cmd)
// 		return ;
// 	free_args(cmd->args);
// 	free_redirects(cmd->redirects);
// 	free(cmd);
// }

void	free_segments(t_segment *segments)
{
	t_segment	*temp;
	t_token		*token_temp;
	t_token		*token_next;

	while (segments)
	{
		temp = segments;
		segments = segments->next;
		while (temp->tokens)
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

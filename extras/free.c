/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 00:26:31 by beysonme         ###   ########.fr       */
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

void	free_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

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

void	free_commands(t_command *cmd)
{
	t_command	*tmp;

	while (cmd != NULL)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->args)
			free_args(tmp->args);
		if (tmp->redirects)
			free_redirects(tmp->redirects);
		free(tmp);
	}
}

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

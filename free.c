/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:35:13 by kikiz            ###   ########.fr       */
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

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*temp;

	while (redirects)
	{
		temp = redirects;
		redirects = redirects->next;
		free(temp->filename);
		free(temp);
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_args(cmd->args, cmd->argc);
	free_redirects(cmd->redirects);
	free(cmd);
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

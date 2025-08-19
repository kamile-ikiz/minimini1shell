/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 15:47:22 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_parser_error(t_parser *parser)
// {
//     if (parser && parser->error)
//     {
//         if (parser->error_msg)
// 		{
// 			ft_putstr_fd(parser->tokens->value, 2);
// 			ft_putendl_fd(parser->error_msg, 2);
// 		}
//         else
// 			ft_putendl_fd("Parser error: Unknown error", 2);
//     }
// }

void free_tokens(t_token *tokens)
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
// void free_parser(t_parser *parser)
// {
//     if (!parser)
//         return;
//     free_tokens(parser->tokens);
//     if (parser->inp)
//         free(parser->inp);
//     if (parser->error_msg)
//         free(parser->error_msg);
//     free(parser);
// }

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

void free_segments(t_segment *segments)
{
	t_segment	*temp;
	t_token		*token_temp;
	t_token		*token_next;

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

void	free_pipeline(t_command *pipeline)
{
	t_command	*current;
	t_command	*next;

	current = pipeline;
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}


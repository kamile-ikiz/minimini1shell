/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:09:15 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 17:07:34 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_parser_error(parser_t *parser)
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

void free_tokens(token_t *tokens)
{
	token_t	*next;

    while (tokens)
	{
        next = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = next;
    }
}
// void free_parser(parser_t *parser)
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

void	free_pipeline(command_t *pipeline)
{
	command_t	*current;
	command_t	*next;

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


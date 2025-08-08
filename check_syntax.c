/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:03:39 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/08 20:21:06 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax  near unexpected token ", 2);
	ft_putendl_fd(token, 2);
}

token_t	*get_token_at_index(token_t *head, int index)
{
	int		i;
	token_t	*current;

	if (!head || index < 0)
		return (NULL);
	current = head;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	return (current);
}

int	get_token_count(token_t *head)
{
	int		count;
	token_t	*current;

	if (!head)
		return (0);
	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	check_pipe_start_end(token_t *head, int token_count)
{
	token_t	*last;

	if (token_count == 0)
		return (1);
	if (head->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	last = get_token_at_index(head, token_count - 1);
	if (last && last->type == TOKEN_PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

int check_consecutive_pipes(token_t *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next->type == TOKEN_PIPE)
		{
			print_syntax_error("|");
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

int	check_pipe_syntax(token_t *tokens, int token_count)
{
		if (!check_pipe_start_end(tokens, token_count))
		{
			return (0);
		}
		if (!check_consecutive_pipes(tokens))
			return (0);
		tokens = tokens->next;
	return (1);
}

int	check_redirect_in_syntax(token_t *head)
{
	token_t	*current;

	current = head;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (current->next->type != TOKEN_WORD)
			{
				print_syntax_error("<");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_redirect_out_syntax(token_t *head)
{
	token_t	*current;

	current = head;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (current->next->type != TOKEN_WORD)
			{
				print_syntax_error(">");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_redirect_append_syntax(token_t *head)
{
	token_t	*current;

	current = head;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_APPEND)
		{
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (current->next->type != TOKEN_WORD)
			{
				print_syntax_error(">>");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_heredoc_syntax(token_t *head)
{
	token_t	*current;

	current = head;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (!current->next)
			{
				print_syntax_error("newline");
				return (0);
			}
			if (current->next->type != TOKEN_WORD)
			{
				print_syntax_error("<<");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_all_syntax(token_t *head)
{
    int token_count;

    token_count = get_token_count(head);
	if (!check_pipe_syntax(head, token_count))
		return (0);
	if (!check_redirect_in_syntax(head))
		return (0);
	if (!check_redirect_out_syntax(head))
		return (0);
	if (!check_redirect_append_syntax(head))
		return (0);
	if (!check_heredoc_syntax(head))
		return (0);
	return (1);
}

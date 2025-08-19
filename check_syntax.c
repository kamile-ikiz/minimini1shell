/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:03:39 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 15:22:13 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirect_in_syntax(t_token *head)
{
	t_token	*current;

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

int	check_redirect_out_syntax(t_token *head)
{
	t_token	*current;

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

int	check_redirect_append_syntax(t_token *head)
{
	t_token	*current;

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

int	check_heredoc_syntax(t_token *head)
{
	t_token	*current;

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

int	check_all_syntax(t_token *head)
{
	int	token_count;

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
